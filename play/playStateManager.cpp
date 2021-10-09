#include "playStateManager.h"
#include "../view/controlViewManager.h"
#include "../view/mapViewManager.h"
#include "../view/miniMapViewManager.h"
#include "../view/statsViewManager.h"

using namespace Play;

const bool REGEN_MAP = false;
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
    using namespace View;
    _controlView = new ControlViewManager(renderer(), CONTROL_VIEW, assets());
    _miniMapView = new MiniMapViewManager(renderer(), MINIMAP_VIEW, assets());
    _statsView = new StatsViewManager(renderer(), STATS_VIEW, assets());
    _mapView = new MapViewManager(renderer(), MAP_VIEW, assets());
}

PlayStateManager::~PlayStateManager()
{
    deletePtr(_map);
    deletePtr(_controlView);
    deletePtr(_miniMapView);
    deletePtr(_statsView);
    deletePtr(_mapView);
}

/**
 * Sets up graphics then Starts the main loop for this state.
 * @returns the state the core loop should be in when the PlayState ends.
 */
Core::CoreState PlayStateManager::start(Party& party)
{
    state(PlayState::Movement);
    result(Core::CoreState::Exit);

    CombatManager combatManager = CombatManager(renderer(), assets(), View::ScreenViewContainer{_controlView, _miniMapView, _statsView, _mapView});
    MenuManager menuManager = MenuManager(renderer(), assets());

    // Create a simple 5x5 map for testing.
    if (REGEN_MAP)
    {
        std::vector<Persistence::MapFileBlock> file = tempMapFile();
        writeMapFile("maps/map1_1", 20, 13, &file);
    }

    // Load the map
    _map = loadMap();

    // No PC, what's the point - Ollies Outie
    if (_map->contents().at(0) == nullptr)
        return Core::CoreState::Exit;

    _map->party(party);

    bool rerender = true;

    // Enter the main loop.
    while(state() != PlayState::Exit)
    {
        // Free up the CPU to do other shit each iteration.
        //if (!rerender)
          //  Util::sleep(50);

        if(_map->party()->isDefeated())
        {
            // _state = GameOver
            state(PlayState::Exit);
            break;
        }

        _map->buryTheDead();

        if (rerender)
            render();
        rerender = false;

        switch(state())
        {
            case PlayState::Message:
                rerender = processMessage();
                continue;
            case PlayState::Menu:
                state(menuManager.start(party));
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
                exit(Core::CoreState::Title);
            default:
                exit(Core::CoreState::Exit);
        }
    }

    return result();
}

/**
 * Shows a message if one exists and waits for a button press.
 */
bool PlayStateManager::processMessage(void)
{
    SDL_Event event;
    PlayState oldState = state();
    Util::sleep(50);
    bool result = false;
    Event myEvent;
    while(eventManager.pollEvent(&myEvent) && oldState == state())
    {
        SDL_Event event = *myEvent.InnerEvent;
        switch(event.type)
        {
            case SDL_QUIT:
                _message = "";
                exit();
                return false;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                   case SDLK_LEFTBRACKET:
                   case SDLK_RIGHTBRACKET:
                        result = true;
                        if (_message.length() < _controlView->lastDrawnCharCount())
                            _message = "";
                        else
                            _message = _message.substr(_controlView->lastDrawnCharCount());
                        break;
                }
        }
    }
    if (_message == "")
        state(PlayState::Movement);

    return result;
}

/**
 * Processes input and other events while the PC is moving around the map.
 * @return true if we need to rerender, otherwise false.
 */
bool PlayStateManager::processMovementState(void)
{
    SDL_Event event;
    MapObject* pc = _map->party();
    bool hasUpdate = false;

    PlayState oldState = state();

    // Enemies should move around the map.
    int time = SDL_GetTicks();
    for(MapObject* m : _map->contents())
    {
        if (!m->isMob())
            continue;
        Mob* mob = (Mob*) m;

        if (mob->type() != MobType::Hostile)
            continue;
        if (!mob->tryUnblock(time) && mob->isBlocked())
            continue;

        Enemy* nme = (Enemy*) mob;
        hasUpdate |= nme->aiMove(*_map);
    }

    Event myEvent;
    while(eventManager.pollEvent(&myEvent) && oldState == state())
    {
        SDL_Event event = *myEvent.InnerEvent;
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
                    case SDLK_LEFTBRACKET:
                        hasUpdate = processCancel();
                        break;
                    case SDLK_RIGHTBRACKET:
                        hasUpdate = processInspectCommand(_map->party());
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

    for(MapObject* m : _map->contents())
    {
        if (!m->isMob())
            continue;

        Mob* enemy = (Mob*) m;

        // This is kind of just a band-aid.....
        if (enemy->isPlayerControlled())
            continue;

        if (enemy->isSeen(*_map->party()))
            state(PlayState::Combat);
    }

    return hasUpdate;
}

bool PlayStateManager::processInspectCommand(Party* party)
{
    int x = party->x();
    int y = party->y();
    switch(party->facing())
    {
        case Direction::NORTH:
            y--; break;
        case Direction::SOUTH:
            y++; break;
        case Direction::EAST:
            x++; break;
        case Direction::WEST:
            x--; break;
        default: break;
    }

    PlayStateContainer data = buildEventData();
    writeBackEventData(
        _map->onInspect(x, y, data)
    );

    return true;
}

/**
 * Builds an object that can be passed along to event handlers deep in the map.
 */
PlayStateContainer PlayStateManager::buildEventData(void) const
{
    PlayStateContainer data;
    data.Message = _message;
    data.State = state();
    data.Map = _map;
    return data;
}

/**
 * If data was changed by event handlers, write it back to the manager.
 */
void PlayStateManager::writeBackEventData(const PlayStateContainer& data)
{
    _message = data.Message;
    if (data.State == state() && _message.length() > 0)
        state(PlayState::Message);
    else
        state(data.State);
}

bool PlayStateManager::processCancel(void)
{
    if (_message.size() > 0)
    {
        _message = "";
        return true;
    }
    return false;
}

/**
 * Moves the mob one cell in the requested direction, if possible.
 * @param mob The mob to be moved.
 * @param input The direction that was input.
 * @return true if move succeeded.
 */
bool PlayStateManager::moveMob(MapObject* mob, Core::InputPress input)
{
    int x = mob->x();
    int y = mob->y();
    switch(input)
    {
        case Core::InputPress::UP:
            mob->facing(Direction::NORTH);
            y--; break;
        case Core::InputPress::LEFT:
            mob->facing(Direction::WEST);
            x--; break;
        case Core::InputPress::DOWN:
            mob->facing(Direction::SOUTH);
            y++; break;
        case Core::InputPress::RIGHT:
            mob->facing(Direction::EAST);
            x++; break;
    }

    PlayStateContainer data = buildEventData();
    bool result = _map->moveMob(mob, x, y, &data);
    writeBackEventData(data);

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
    GameMap* gameMap = new GameMap();
    gameMap->loadChunk(1, 1, "maps/map1_1");
    gameMap->requestNextChunk();
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
    _controlView->render(_map->party()->leader(), state(), _message);
    _statsView->render(*_map, state());
    _miniMapView->render();

    SDL_RenderPresent(renderer());
}

/**
 *
 */
void PlayStateManager::writeMapFile(const std::string& fileName, const int width, const int height, const std::vector<Persistence::MapFileBlock>* data)
{
    using namespace Persistence;

    int fileSize =  (data->size() * MapFileBlock::BYTES_PER_CELL)+2;
    std::vector<byte> dataBytes = std::vector<byte>(0);
    dataBytes.reserve(fileSize);

    dataBytes.push_back(width);
    dataBytes.push_back(height);

    for (auto const & cell : *data)
    {
        dataBytes.push_back(byte(cell.terrainType));
        dataBytes.push_back(byte(cell.mobType));
    }

    Util::writeFile(fileName, dataBytes);
}

// Test data.
std::vector<Persistence::MapFileBlock> PlayStateManager::tempMapFile()
{
    using namespace Persistence;
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
        MapFileBlock::generateTestCell(GrassTerrain, MobType::PartyOfMobs),
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
        //MapFileBlock::generateTestCell(GrassTerrain, MobType::Hostile),
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
        MapFileBlock::generateTestCell(WallTerrain)
    };

    return result;
}

