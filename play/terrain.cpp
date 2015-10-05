#include "terrain.h"
#include "playStateManager.h"

using namespace Play;

//{ Lifecycle
Resources::MapObjectTemplate getTerrainDefaultTemplate(void)
{
    Resources::MapObjectTemplate result;
    result.ImagePath = "";
    result.IsDense = true;
    return result;
}

/**
 * Empty Constructor
 */
Terrain::Terrain()
    :MapObject(getTerrainDefaultTemplate())
{}

/**
 * Constructor
 * @param type
 */
Terrain::Terrain(const Resources::TerrainTemplate& tmpl)
    :MapObject(tmpl)
{
    _onEnter = tmpl.OnEnter;
}

//}

//{ Methods

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

//}
