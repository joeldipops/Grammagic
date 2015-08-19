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
Terrain::Terrain(TerrainType type)
    :MapObject(Templates::MapObjectTemplate{ "", false, nullptr})
{
    _terrainType = type;
    switch(_terrainType)
    {
        case GrassTerrain:
            isDense(false);
            imageFileName(RESOURCE_LOCATION + "grass.png");
            break;
        case WallTerrain:
            isDense(true);
            imageFileName(RESOURCE_LOCATION + "wall.png");
            break;
        default:
            isDense(false);
            imageFileName(RESOURCE_LOCATION + "hidden.png");
            break;
    }
}

/**
 * Gets the type of terrain.
 */
TerrainType Terrain::type(void) const { return _terrainType; }

PlayStateContainer& Terrain::onInspect(PlayStateContainer& data)
{
    return data;
}
