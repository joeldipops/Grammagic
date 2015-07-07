#ifndef MAPCELL_H_INCLUDED
#define MAPCELL_H_INCLUDED

#include "terrain.h"
#include "mob.h"

class MapCell
{
    public:
        MapCell();
        MapCell(TerrainType);
        Terrain* terrain(Terrain* = nullptr);
        const Terrain* terrain(void) const;
        Mob* contents(Mob* = nullptr);
        void empty(void);

    private:
        Terrain _terrain;
        Mob* _contents;
};

#endif
