#include "gameMap.h"

using namespace Play;

//{LIFE CYCLE
/**
 * Contructs a rectangular map with the given dimensions.
 */
GameMap::GameMap()
{
    _contents = std::vector<MapObject*>(0);
    // Reserve the first spot for the PC mob.
    _cells = std::map<unsigned long, MapCell>();
}

/**
 * Destructor.
 */
GameMap::~GameMap()
{
    // One as PC is a special case
    for(natural i = 1; i < _contents.size(); i++)
        kill(_contents.at(i));
    _contents = std::vector<MapObject*>(0);
}
//}

//{ Properties

/**
 * Gets or sets the list of mobs that exist on this map.
 * @param mobs
 * @return
 */
std::vector<MapObject*> GameMap::contents(std::vector<MapObject*> contents_) { return _contents = contents_; }
std::vector<MapObject*> GameMap::contents(void) const { return _contents; }
//}

//{METHODS
int GameMap::width(void) const
{
    return _maxX * CHUNK_WIDTH + CHUNK_WIDTH;
}

int GameMap::height(void) const
{
    return _maxY * CHUNK_HEIGHT + CHUNK_HEIGHT;
}

bool GameMap::requestNextChunk(void)
{
    int x = party()->x() / CHUNK_WIDTH;
    int y = party()->y() / CHUNK_HEIGHT;
    SDL_Rect v = visible();

    const int minVisibleX = v.x / CHUNK_WIDTH;
    const int minVisibleY = v.y / CHUNK_HEIGHT;
    const int maxVisibleX = (v.w) / CHUNK_WIDTH;
    const int maxVisibleY = (v.h) / CHUNK_HEIGHT;

    // Keep the chunk we're standing in.
    loadChunk(x, y);

    if (minVisibleX < x)
    {
        loadChunk(minVisibleX, y);
        if (minVisibleY < y)
            loadChunk(minVisibleX, minVisibleY);
        if (maxVisibleY > y)
            loadChunk(minVisibleX, maxVisibleY);
    }
    if (minVisibleY < y)
    {
        loadChunk(x, minVisibleY);
        if (maxVisibleX > x)
            loadChunk(maxVisibleX, maxVisibleY);
    }
    if (maxVisibleX > x)
    {
        loadChunk(maxVisibleX, y);
        if (maxVisibleY > y)
            loadChunk(maxVisibleX, maxVisibleY);
    }
    if (maxVisibleY > y)
        loadChunk(x, maxVisibleY);

    return false;
}

bool GameMap::loadChunk(int cX, int cY)
{
    const std::string path = "maps/map";
    return loadChunk(cX, cY, path + std::to_string(cX) + "_" + std::to_string(cY));
}

/**
 * Loads a chunk of map from file the resizes & reindexes the in-memory map to add it.
 * @param x The x co-ordinate of the global map chunk.
 * @param y The y co-ordinate of the global map chunk.
 * @param path Path to the file where the chunk is stored.
 */
bool GameMap::loadChunk(int cX, int cY, const std::string& path)
{
    using namespace Persistence;

    if (cX < 0 || cY < 0)
        return false;

    bool wasFound = false;
    // Check if this chunk is already in memory.
    for (natural i = 0; i < _chunks.size(); i++)
    {
        SDL_Rect chunk = _chunks.at(i);
        if (chunk.x == cX && chunk.y == cY)
        {
            wasFound = true;
            // if it's already at the front of the queue, then we're good.
            if (i != 0)
            {
                // bring to front of queue.
                _chunks.erase(_chunks.begin() + i);
                _chunks.push_front(chunk);
            }
        }
        // We already have this chunk so we don't need to do anything else.
        if (wasFound)
            return false;
    }

    std::vector<byte> mapData = Util::readFile(path);
    if (!mapData.size())
        return false;

    // Clean up
    if (_chunks.size() >= CHUNK_LIMIT)
    {
        SDL_Rect toRemove = _chunks.back();
        removeChunk(toRemove.x, toRemove.y);
        _chunks.pop_back();
    }

    // Add new chunk to the queue.
    _chunks.push_front(SDL_Rect {cX, cY, 0, 0});

    // find extremes
    int oldMaxX = _maxX;
    int oldMaxY = _maxY;
    _maxX = _maxY = 0;
    for (const SDL_Rect& chunk : _chunks)
    {
        if (chunk.x > _maxX)
            _maxX = chunk.x;
        if (chunk.y > _maxY)
            _maxY = chunk.y;
    }

    // Reindex if necessary.
    if (_maxX > oldMaxX)
    {
        const int newWidth = _maxX * CHUNK_WIDTH + CHUNK_WIDTH;
        const int oldWidth = oldMaxX * CHUNK_WIDTH + CHUNK_WIDTH;
        const int oldHeight = oldMaxY * CHUNK_HEIGHT + CHUNK_HEIGHT;

        std::map<unsigned long, MapCell> temp;
        for (int y = 0; y < oldHeight; y++)
        {
            for (int x = 0; x < oldWidth; x++)
            {
                natural index = x + (oldWidth * y);
                if (_cells.count(index) <= 0)
                    continue;

                MapCell cell = _cells.at(index);
                temp[x + (newWidth * y)] = cell;
            }
        }

        _cells = temp;
    }
    // I don't know if ultimately I want to do it this way, or if I really should reindex
    // when the map shrinks, but fuck it, it'll fix my immediate bug.
    else
        _maxX = oldMaxX;

    // - 1 to avoid some text-file artifact I'm getting
    for (natural i = 0; i < mapData.size() -1; i+= MapFileBlock::BYTES_PER_CELL)
    {
        int position = i / MapFileBlock::BYTES_PER_CELL;
        int x = (position % CHUNK_WIDTH) + cX * CHUNK_WIDTH;
        int y = (position / CHUNK_WIDTH) + cY * CHUNK_HEIGHT;

        MapCell cell;
        switch (TerrainType(mapData[i]))
        {
            case TerrainType::GrassTerrain:
                cell = MapCell(Resources::Data::Grass);
                break;
            case TerrainType::WallTerrain:
                cell = MapCell(Resources::Data::Wall);
                break;
            case TerrainType::HutTerrain:
                cell = MapCell(Resources::Data::Hut);
                break;
            case TerrainType::CacheTerrain:
                cell = MapCell(Resources::Data::Cache);
            default:
                break;
        }

        setCell(x, y, &cell);

        MobType contents = MobType(mapData[i+1]);

        if (contents == MobType::None)
            continue;

        MapObject* mob = nullptr;
        switch(contents)
        {
            case MobType::NPC1:
                mob = new NPC(Resources::Data::NPC1);
                break;
            case MobType::E2:
                mob = new Enemy(Resources::Data::E2);
                break;
            case MobType::B1:
                mob = new Enemy(Resources::Data::B1);
                break;
            case MobType::E1:
            case MobType::Hostile:
                mob = new Enemy(Resources::Data::E1);
                break;
            case MobType::PartyOfMobs:
            case MobType::PlayerCharacter: {
                if (party() == nullptr)
                    mob = new Party();
                break;
            }
            default:
                break;
        }
        // If the mob has already been assigned a position place it there.
        if (mob != nullptr)
            place(mob, x, y);
    }

    return true;
}

/**
 * Places a mob on to the map at the specified position.
 * @param mob the mob.
 * @param x
 * @param y
 * @return true if placed successfuly, false otherwise.
 */
bool GameMap::place(MapObject* mob, int x, int y, bool canReplace)
{
    MapCell* cell = getCell(x, y);

    if (cell == nullptr)
        return false;

    if (mob->isPlayerParty())
    {

        if (_contents.size() > 0 && _contents.at(0)->isPlayerParty())
        {
            kill(_contents.at(0));
        }

        _contents.insert(_contents.begin(), mob);
    }
    else
    {
        if (cell->contents() != nullptr)
        {
            if (canReplace)
                kill(cell->contents());
            else
                return false;
        }
        _contents.push_back(mob);
    }


    mob->location(x, y);
    cell->contents(mob);

    return true;
}


/**
 * Gets the current pc, or places a new PC
 * @param pc
 * @return
 */
Party* GameMap::party(Party& party_)
{
    if (_contents[0] != nullptr)
    {
        MapObject* old = _contents[0];
        int newX = (party_.x() >= 0) ? party_.x() : old->x();
        int newY = (party_.y() >= 0) ? party_.y() : old->y();

        kill(old);

        this->place((MapObject*)&party_, newX, newY, true);
    }

    _contents[0] = &party_;
    return (Party*) _contents.at(0);
}

Party* GameMap::party(void) const
{
    if (_contents.size() <= 0)
        return nullptr;

    return (Party*) _contents.at(0);

}

/**
 * Move a mob on the map from one cell to another.
 * @param mob The mob to move.
 * @param loc The location to move to.
 * @return true if move was successful, false otherwise
 */
bool GameMap::moveMob(MapObject* mob, Location loc)
{
    return moveMob(mob, loc.X, loc.Y);
}

PlayStateContainer& GameMap::onInspect(int x, int y, PlayStateContainer& data)
{
    return getCell(x, y)->onInspect(data);
}

/**
 * removes a mob from the map.
 */
void GameMap::remove(MapObject* mob)
{
    // Remove from the map.
    MapCell* cell = getCell(mob->x(), mob->y());
    if (cell != nullptr)
        cell->empty();

    for (natural i = 0; i < _contents.size(); i++)
    {
        if (mob == _contents.at(i))
            _contents.erase(_contents.begin() + i);
    }
}

/**
 * Remove a mob from the map, and from existence
 * @param Mob the Mob to remove.
 */
void GameMap::kill(MapObject* mob)
{
    remove(mob);
    delete mob;
}

/**
 * Removes any mob from the map and from existence if they have 0 stamina.
 */
void GameMap::buryTheDead(void)
{
    MapObject* m;
    for(natural i = 0; i < _contents.size(); i++)
    {
        party()->buryTheDead();
        m = _contents.at(i);
        if (!m->isMob())
            continue;
        Mob* mob = (Mob*) m;
        if (mob->stamina() <= 0)
        {
            // Remove from the map.
            getCell(mob->x(), mob->y())->empty();
            // Remove from existence
            _contents.erase(_contents.begin() + i);
            i--;
            if (!mob->isPlayerControlled())
                delete mob;
        }
    }
}

/**
 * Gets information on a specific cell on the map.
 * @param x
 * @param y
 * @return
 */
MapCell* GameMap::getCell(int x, int y)
{
    natural index = x + (y * width());
    if (_cells.count(index) > 0)
        return &_cells.at(index);

    return nullptr;
}

const MapCell* GameMap::getCell(int x, int y) const
{
    natural index = x + (y * width());
    if (_cells.count(index) >= 1)
        return &_cells.at(index);

    return nullptr;
}

void GameMap::setCell(int x, int y, MapCell* value)
{
    natural index = x + (y * width());
    value->terrain()->location(x, y);
    if (value->contents() != nullptr)
        value->contents()->location(x, y);
    _cells[index] = *value;
}

/**
 * Frees up memory
 * @param x of the chunk
 * @param y of the chunk
 */
void GameMap::removeChunk(int cX, int cY)
{
    int startX = cX * CHUNK_WIDTH;
    int startY = cY * CHUNK_HEIGHT;

    for (natural y = startY; y < startY + CHUNK_HEIGHT; y++)
    {
        for (natural x = startX; x < startX + CHUNK_WIDTH; x++)
        {
            int index = x + (y * width());
            if (_cells.count(index) >= 1)
            {
                if (_cells.at(index).contents() != nullptr)
                {
                    MapObject* d = _cells.at(index).contents();
                    kill(d);
                }

                _cells.erase(index);
            }
        }
    }
}

/**
 *
 * @param mob
 * @param x
 * @param y
 * @return true if move was successful, false otherwise
 */
bool GameMap::moveMob(MapObject* mob, int x, int y, PlayStateContainer* data)
{
    if (x < 0 || x >= width())
        return false;
    if (y < 0 || y >= height())
        return false;

    MapCell* cell = getCell(x, y);
    if (cell == nullptr)
        return false;

    if (cell->terrain()->isDense())
        return false;

    if (cell->contents() != nullptr && cell->contents()->isDense())
        return false;

    int oldX = mob->x();
    int oldY = mob->y();
    mob->location(x, y);
    cell->enter(mob, data);
    getCell(oldX, oldY)->empty();

    if (mob->isPlayerParty())
        requestNextChunk();
    return true;
}

/**
 * Returns a rect representing the area of the map that is currently on screen
 * (sorta).
 */
const SDL_Rect GameMap::visible(void) const
{
    int x = party()->x() - int(HORIZONTAL_VISION);
    int y = party()->y() - int(VERTICAL_VISION);
    return SDL_Rect
    {
        x,
        y,
        x + 1 + int(HORIZONTAL_VISION) * 2,
        y + 1 + int(VERTICAL_VISION) * 2
    };

}
//}
