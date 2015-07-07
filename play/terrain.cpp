#include "terrain.h"

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
            imageFileName(RESOURCE_LOCATION + "blank.png");
            break;
    }
}

/**
 *
 * @return
 */
TerrainType Terrain::Type(void)
{
    return _terrainType;
}

/**
 *
 * @param type
 * @return
 */
TerrainType Terrain::Type(TerrainType type)
{
    _terrainType = type;
    return _terrainType;
}
