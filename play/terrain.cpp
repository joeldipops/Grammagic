#include "terrain.h"
#include "playStateManager.h"

using namespace Play;

/**
 * Empty Constructor
 */
Terrain::Terrain()
    :MapObject(Templates::MapObjectTemplate{"", true, nullptr})
{}

/**
 * Constructor
 * @param type
 */
Terrain::Terrain(const Templates::TerrainTemplate& tmpl)
    :MapObject(tmpl)
{
    _onEnter = tmpl.OnEnter;
}

PlayStateContainer& Terrain::onInspect(PlayStateContainer& data)
{
    if (onInspectFn() != nullptr)
        return onInspectFn()(this, data);

    return data;
}

PlayStateContainer& Terrain::onEnter(PlayStateContainer& data)
{
    if (_onEnter != nullptr)
        return _onEnter(this, data);

    return data;
}
