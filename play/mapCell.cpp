#include "mapCell.h"

MapCell::MapCell(){}

/**
 * Constructor
 * @param terrain The terrain in this cell of the map.
 */
MapCell::MapCell(TerrainType type)
{
    _terrain = Terrain(type);
    _contents = nullptr;
}

/**
 * Sets and gets the terrain in this cell of the map.
 * @param terrain the cell should be set to.
 * @return The current terrain, after the update.
 */
Terrain* MapCell::terrain(Terrain* terrain_)
{
    if (terrain_ != nullptr)
        _terrain = *terrain_;
    return &_terrain;
}
const Terrain* MapCell::terrain(void) const
{
    return &_terrain;
}

/**
 * Sets and gets the mob on this cell of the map.
 * @param mob The mob that's placed on this cell.
 * @param The mob on this cell after the update.
 */
MapObject* MapCell::contents(MapObject* contents_)
{
    if (contents_ != nullptr)
        _contents = contents_;
    return _contents;
}

/**
 * Removes any mobs from this cell.
 */
void MapCell::empty(void)
{
    _contents = nullptr;
}

