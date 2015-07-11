#include "playStateManager.h"

using namespace Play;

const bool REGEN_MAP = true;
const SDL_Rect CONTROL_VIEW = {0, 0, 1000, 150};
const SDL_Rect MINIMAP_VIEW = {1000, 0, 200, 150};
const SDL_Rect STATS_VIEW = {1000, 150, 200, 650};
const SDL_Rect MAP_VIEW = {0, 150, 1000, 650};
const int COMBAT_GRACE_PERIOD = 1000;

/**
 * Constructor
 */
PlayStateManager::PlayStateManager(SDL_Renderer* r, AssetCache* a) : StateManager(r, a)
{
    _controlView = new ControlViewManager(renderer(), CONTROL_VIEW, assets());
    _miniMapView = new MiniMapViewManager(renderer(), MINIMAP_VIEW, assets());
    _statsView = new StatsViewManager(renderer(), STATS_VIEW, assets());
    _mapView = new MapViewManager(renderer(), MAP_VIEW, assets());
}

PlayStateManager::~PlayStateManager()
{
    delete _controlView;
    delete _map;
    _controlView = nullptr;
}

/**
 * Sets up graphics then Starts the main loop for this state.
 * @returns the state the core loop should be in when the PlayState ends.
 */
Core::CoreState PlayStateManager::start(void)
{
    state(PlayState::Movement);
    result(Core::CoreState::Exit);

    CombatManager combatManager = CombatManager(renderer(), assets(), ScreenViewContainer{_controlView, _miniMapView, _statsView, _mapView});
    MenuManager menuManager = MenuManager(renderer(), assets());

    // Create a simple 5x5 map for testing.
    if (REGEN_MAP)
    {
        std::vector<MapFileBlock> file = tempMapFile();
        writeMapFile("map", 20, 13, &file);
    }

    // Load the map
    _map = loadMap();

    // No PC, what's the point - Ollies Outie
    if (_map->mobs().at(0)->type() != MobType::PC)
        return Core::CoreState::Exit;

    bool rerender = true;

    // Enter the main loop.
    while(state() != PlayState::Exit)
    {
        // Free up the CPU to do other shit each iteration.
        //Util::sleep(20);
        if (!rerender)
            ;

        if (_map->pc()->stamina() <= 0)
        {
            // _state = GameOver
            state(PlayState::Exit);
            break;
        }

        _map->buryTheDead();


        if (rerender)
            render();

        switch(state())
        {
            case PlayState::Menu:
                state(menuManager.start(_map->pc()));
                continue;
            case PlayState::Victory:
                state(PlayState::Movement);
            case PlayState::Movement:
                rerender = processMovementState();
                continue;
            case PlayState::Combat:
                state(combatManager.start(_map));
                // We shouldn't start another battle as soon as one ends.
                _combatGraceTime = SDL_GetTicks() + COMBAT_GRACE_PERIOD;
                continue;
            case PlayState::GameOver:
            default:
                exit(Core::CoreState::Exit);
        }
    }

    switch (state()) {
        case PlayState::GameOver:
        case PlayState::Exit:
            result(Core::CoreState::Exit); break;
        default:
            result(Core::CoreState::Title); break;
    }

    return result();
}

/**
 * Processes input and other events while the PC is moving around the map.
 * @return true if we need to rerender, otherwise false.
 */
bool PlayStateManager::processMovementState(void)
{
    SDL_Event event;
    Mob* pc = _map->pc();
    bool hasUpdate = false;

    PlayState oldState = state();

    // Enemies should move around the map.
    int time = SDL_GetTicks();
    for(Mob* mob : _map->mobs())
    {
        if (mob->type() != MobType::Hostile)
            continue;
        if (!mob->tryUnblock(time) && mob->isBlocked())
            continue;

        Enemy* nme = (Enemy*) mob;
        hasUpdate |= nme->aiMove(_map);
    }

    while(SDL_PollEvent(&event) != 0 && oldState == state())
    {
        switch(event.type)
        {
            case SDL_QUIT:
                exit();
                return false;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_q:
                    case SDLK_ESCAPE:
                        exit(Core::CoreState::Title);
                        return false;
                    case SDLK_w:
                        hasUpdate = moveMob(pc, Core::InputPress::UP);
                        break;
                    case SDLK_a:
                        hasUpdate = moveMob(pc, Core::InputPress::LEFT);
                        break;
                    case SDLK_s:
                        hasUpdate = moveMob(pc, Core::InputPress::DOWN);
                        break;
                    case SDLK_d:
                        hasUpdate = moveMob(pc, Core::InputPress::RIGHT);
                        break;
                    case SDLK_RETURN:
                        hasUpdate = false;
                        state(PlayState::Menu);
                        break;
                }
        }
    }

    // After combat ends, you have a few moments to get away.
    if (time < _combatGraceTime)
        return hasUpdate;

    std::vector<Mob*> enemies = _map->mobs();
    for(int i = 1; i < int(enemies.size()); i++)
    {
        if (enemies.at(i)->isSeen(_map->pc()))
            state(PlayState::Combat);
    }

    return hasUpdate;
}

/**
 * Moves the mob one cell in the requested direction, if possible.
 * @param mob The mob to be moved.
 * @param input The direction that was input.
 * @return true if move succeeded.
 */
bool PlayStateManager::moveMob(Mob* mob, Core::InputPress input)
{
    int x = mob->x();
    int y = mob->y();
    switch(input)
    {
        case Core::InputPress::UP:
            y--; break;
        case Core::InputPress::LEFT:
            x--; break;
        case Core::InputPress::DOWN:
            y++; break;
        case Core::InputPress::RIGHT:
            x++; break;
    }

    bool result = _map->moveMob(mob, x, y);

    return result;
}

void PlayStateManager::exit(const Core::CoreState nextState)
{
    state(PlayState::Exit);
    result(nextState);
}

/**
 * Loads and parses the map file.
 * @return The parsed Map.
 */
GameMap* PlayStateManager::loadMap(void)
{
    std::vector<char> mapData = readFile("map");
    int length = mapData.size();
    GameMap* gameMap = new GameMap(mapData[0], mapData[1]);

    for (int i = 2; i < length; i+= MapFileBlock::BYTES_PER_CELL)
    {
        int width = gameMap->width();

        int position = (i - 2) / 2;
        int x = position % width;
        int y = position / width;

        gameMap->setCell(x, y, new MapCell(TerrainType(mapData[i])));

        MobType contents = MobType(mapData[i+1]);
        if (contents == MobType::None)
            continue;

        Mob* mob;
        if(contents == MobType::Hostile)
            mob = new Enemy();
        else
            mob = new Mob(contents);

        gameMap->placeMob(mob, x, y);
    }
    gameMap->mobs().shrink_to_fit();
    return gameMap;
}

/**
 * Paints the visual elements on the screen with SDL
 */
void PlayStateManager::render()
{
    SDL_SetRenderDrawColor(renderer(), 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer());

    _mapView->render(_map, state());
    _controlView->render(_map->pc(), state());
    _statsView->render(_map, state());
    _miniMapView->render();

    SDL_RenderPresent(renderer());
}

/**
 * Reads a file and returns an array of bytes.
 * @param fileName the file to read.
 * @return The file's contents as an array of bytes.
 */
std::vector<char> PlayStateManager::readFile(const char* fileName)
{
    std::vector<char> result ;
    std::streampos size;
    std::ifstream mapFile(fileName, std::ios::in|std::ios::binary|std::ios::ate);

    if (!mapFile.is_open())
        return std::vector<char>();

    size = mapFile.tellg();

    char* data = new char[size];
    mapFile.seekg(0, std::ios::beg);
    mapFile.read(data, size);
    mapFile.close();

    result.assign(data, data+size);
    return result;
}

/**
 *
 */
void PlayStateManager::writeMapFile(const char* fileName, const int width, const int height, const std::vector<MapFileBlock>* data)
{
    int fileSize =  (data->size() * MapFileBlock::BYTES_PER_CELL)+2;
    char* dataBytes = new char[fileSize];

    dataBytes[0] = width;
    dataBytes[1] = height;
    int position = 2;

    for (auto const &cell : *data)
    {
        dataBytes[position] = char(cell.terrainType);
        dataBytes[position+1] = char(cell.mobType);
        position += MapFileBlock::BYTES_PER_CELL;
    }

    std::ofstream mapFile(fileName, std::ofstream::binary);
    mapFile.write(dataBytes, fileSize);
    mapFile.close();

    delete dataBytes;
}

// Test data.
std::vector<MapFileBlock> PlayStateManager::tempMapFile()
{
    std::vector<MapFileBlock> result = std::vector<MapFileBlock> {
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),

        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(GrassTerrain, MobType::PC),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(WallTerrain),

        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(WallTerrain),

        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(WallTerrain),

        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(WallTerrain),

        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain, MobType::Hostile),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(WallTerrain),

        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(WallTerrain),

        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain, MobType::Hostile),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(WallTerrain),

        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain, MobType::Hostile),
        MapFileBlock::generateTestCell(GrassTerrain, MobType::Hostile),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(WallTerrain),

        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(WallTerrain),

        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain, MobType::Hostile),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(WallTerrain),

        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain, MobType::Hostile),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(GrassTerrain),
        MapFileBlock::generateTestCell(WallTerrain),

        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain, MobType::Hostile),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
        MapFileBlock::generateTestCell(WallTerrain),
    };

    return result;
}

