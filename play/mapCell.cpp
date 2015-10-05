#include "mapCell.h"
#include "playStateManager.h"

//{ Lifecycle
/**
 * Constructor
 * @param terrain The terrain in this cell of the map.
 */
MapCell::MapCell(const Resources::TerrainTemplate& tmpl)
{
    _terrain = Terrain(tmpl);
    _contents = nullptr;
}

/**
 * Destructor
 */
MapCell::~MapCell()
{
    _contents = nullptr;
}

//}


//{ Properties
/**
 * Sets and gets the terrain in this cell of the map.
 * @param terrain the cell should be set to.
 * @return The current terrain, after the update.
 */
const Terrain* MapCell::terrain(void) const { return &_terrain; }
Terrain* MapCell::terrain(Terrain* terrain_)
{
    if (terrain_ != nullptr)
        _terrain = *terrain_;
    return &_terrain;
}

/**
 * Sets and gets the mob on this cell of the map.
 * @param mob The mob that's placed on this cell.
 * @param The mob on this cell after the update.
 */
MapObject* MapCell::contents(void) const { return _contents; }
MapObject* MapCell::contents(MapObject* contents_)
{
    if (contents_ != nullptr)
        _contents = contents_;

    return _contents;
}

//}

//{ Methods

PlayStateContainer* MapCell::enter(MapObject* mob, PlayStateContainer* data)
{
    _contents = mob;

    if (data == nullptr)
        return data;


    PlayStateContainer& result = *data;
    _terrain.onEnter(result);
    data->Message = result.Message;
    data->State = result.State;
    return data;
}


PlayStateContainer& MapCell::onInspect(PlayStateContainer& data)
{
    if (_contents != nullptr)
        return _contents->onInspect(data);
    else
        return _terrain.onInspect(data);
}

/**
 * Removes any mobs from this cell.
 */
void MapCell::empty(void)
{
    _contents = nullptr;
}

//}

