#ifndef TERRAIN_H_INCLUDED
#define TERRAIN_H_INCLUDED

enum TerrainType
{
    WallTerrain,
    GrassTerrain
};

#include "../mapObject.h"

class Terrain : public MapObject
{
    public:
        Terrain(void);
        Terrain(TerrainType);
        TerrainType Type(void);
    protected:
        TerrainType Type(TerrainType);
    private:
        TerrainType _terrainType;
};
#endif
