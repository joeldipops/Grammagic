#include "terrain.h"
#include "playStateManager.h"

using namespace Play;

/**
 * Empty Constructor
 */
Terrain::Terrain(){}

/**
 * Constructor
 * @param type
 */
Terrain::Terrain(TerrainType type)
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
    data.Message = "I'm a terrain";
    return data;
}
