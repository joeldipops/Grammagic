#include "gameMap.h"

using namespace Play;

/**
 * Contructs a rectangular map with the given dimensions.
 * @param width
 * @param height
 */
GameMap::GameMap(int width, int height)
{
    _width = width;
    _height = height;
    _cells = std::vector<MapCell>(width * height);
    _contents = std::vector<MapObject*>(0);
    _contents.reserve(width * height);
    // Reserve the first spot for the PC mob.
    _contents.push_back(nullptr);
}

/**
 * Destructor.
 */
GameMap::~GameMap()
{
    // One as PC is a special case
    for(unsigned int i = 1; i < _contents.size(); i++)
        kill(_contents.at(i));
    _contents = std::vector<MapObject*>(0);
}

/**
 * Get the width of the map in number of cells.
 * @return
 */
int GameMap::width(void) const
{
    return _width;
}

/**
 * Get the height of the map in number of cell.s
 * @return
 */
int GameMap::height(void) const
{
    return _height;
}

/**
 *
 * @param cells
 * @return
 */
std::vector<MapCell>* GameMap::cells(std::vector<MapCell>* cells)
{
    if (cells != nullptr)
        _cells = *cells;
    return &_cells;
}

/**
 * Gets or sets the list of mobs that exist on this map.
 * @param mobs
 * @return
 */
std::vector<MapObject*> GameMap::contents(std::vector<MapObject*> contents_)
{
    _contents = contents_;
    return _contents;
}
std::vector<MapObject*> GameMap::contents(void) const
{
    return _contents;
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
    if (x < 0 || x >= width())
        return false;

    if (y < 0 || y >= height())
        return false;

    MapCell* cell = getCell(x, y);

    if (mob->isPlayerParty())
    {
        delete _contents.at(0);
        _contents.at(0) = mob;
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
        this->place((MapObject*)&party_, old->x(), old->y(), true);
    }

    _contents[0] = &party_;
    return (Party*) _contents.at(0);

}

Party* GameMap::party(void) const
{
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

/**
 * removes a mob from the map.
 */
void GameMap::remove(MapObject* mob)
{
    // Remove from the map.
    MapCell* cell = getCell(mob->x(), mob->y());
    if (cell != nullptr)
        cell->empty();

    for (unsigned int i = 0; i < _contents.size(); i++)
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
    for(unsigned int i = 0; i < _contents.size(); i++)
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
    if (x < 0 || x >= width())
        return nullptr;
    if (y < 0 || y >= height())
        return nullptr;

    unsigned int index = x + (y * width());
    if (index < _cells.size())
        return &_cells.at(index);

    return nullptr;
}

const MapCell* GameMap::getCell(int x, int y) const
{
    if (x < 0 || x >= width())
        return nullptr;
    if (y < 0 || y >= height())
        return nullptr;

    unsigned int index = x + (y * width());
    if (index < _cells.size())
        return &_cells.at(index);

    return nullptr;
}

void GameMap::setCell(int x, int y, MapCell* value)
{
    unsigned int index = x + (y * _width);
    if (index < _cells.size())
        _cells[x + (y * _width)] = *value;
}

/**
 *
 * @param mob
 * @param x
 * @param y
 * @return true if move was successful, false otherwise
 */
bool GameMap::moveMob(MapObject* mob, int x, int y)
{
    if (x < 0 || x >= width())
        return false;

    if (y < 0 || y >= height())
        return false;

    MapCell* cell = getCell(x, y);
    if (cell->terrain()->isDense())
        return false;


    if (cell->contents() != nullptr && cell->contents()->isDense())
        return false;

    int oldX = mob->x();
    int oldY = mob->y();
    mob->location(x, y);
    mob = cell->contents(mob);
    getCell(oldX, oldY)->empty();
    return true;
}
