#include "gameMap.h"

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
    _mobs = std::vector<Mob*>();
    _mobs.reserve(width * height);
    // Reserve the first spot for the PC mob.
    _mobs.push_back(nullptr);
}

GameMap::~GameMap()
{
    for(int i = 0; i < int(_mobs.size()); i++)
        delete _mobs.at(i);
    _mobs = std::vector<Mob*>(0);
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
std::vector<Mob*> GameMap::mobs(std::vector<Mob*> mobs_)
{
    _mobs = mobs_;
    return _mobs;
}
std::vector<Mob*> GameMap::mobs(void) const
{
    return _mobs;
}

/**
 * Places a mob on to the map at the specified position.
 * @param mob the mob.
 * @param x
 * @param y
 * @return true if placed successfuly, false otherwise.
 */
bool GameMap::placeMob(Mob* mob, int x, int y)
{
    if (x < 0 || x >= width())
        return false;

    if (y < 0 || y >= height())
        return false;

    MapCell* cell = getCell(x, y);
    if (cell->contents() != nullptr)
        return false;

    if (mob->type() == MobType::PlayerCharacter)
    {
        delete _mobs.at(0);
        _mobs.at(0) = mob;
    }
    else
        _mobs.push_back(mob);

    mob->location(x, y);
    mob = cell->contents(mob);

    return true;
}

/**
 *
 * @param pc
 * @return
 */
PC* GameMap::pc(PC* pc_)
{
    if (pc_ != nullptr)
        _mobs[0] = pc_;

    return (PC*) _mobs.at(0);
}
const PC* GameMap::pc(void) const
{
    return (PC*) _mobs.at(0);
}

/**
 * Move a mob on the map from one cell to another.
 * @param mob The mob to move.
 * @param loc The location to move to.
 * @return true if move was successful, false otherwise
 */
bool GameMap::moveMob(Mob* mob, Location loc)
{
    return moveMob(mob, loc.X, loc.Y);
}

/**
 * Remove a mob from the map, and from existence
 * @param Mob the Mob to remove.
 */
void GameMap::killMob(const Mob* mob)
{
    // Remove from the map.
    getCell(mob->x(), mob->y())->empty();

    for (int i = 0; i < int(_mobs.size()); i++)
    {
        if (mob == _mobs.at(i))
            _mobs.erase(_mobs.begin() + i);
    }
}

/**
 * Removes any mob from the map and from existence if they ha   ve 0 stamina.
 */
void GameMap::buryTheDead()
{
    Mob* mob;
    for(int i = 0; i < int(_mobs.size()); i++)
    {
        mob = _mobs.at(i);
        if (mob->stamina() <= 0)
        {
            // Remove from the map.
            getCell(mob->x(), mob->y())->empty();
            // Remove from existence
            _mobs.erase(_mobs.begin() + i);
            i--;
        }
    }
}

/**
 *
 * @param x
 * @param y
 * @return
 */
MapCell* GameMap::getCell(int x, int y)
{
    return &_cells[x + (y * _width)];
}
/**
 *
 * @param x
 * @param y
 * @return
 */
const MapCell* GameMap::getCell(int x, int y) const
{
    return &_cells[x + (y * _width)];
}

void GameMap::setCell(int x, int y, MapCell* value)
{
    _cells[x + (y * _width)] = *value;
}

/**
 *
 * @param mob
 * @param x
 * @param y
 * @return true if move was successful, false otherwise
 */
bool GameMap::moveMob(Mob* mob, int x, int y)
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
