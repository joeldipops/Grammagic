#ifndef MAPCELL_H_INCLUDED
#define MAPCELL_H_INCLUDED

#include "terrain.h"
#include "mob.h"

namespace Play
{
    class MapCell
    {
        public:
            MapCell(void);
            MapCell(TerrainType);
            ~MapCell(void);
            Terrain* terrain(Terrain* = nullptr);
            const Terrain* terrain(void) const;
            MapObject* contents(MapObject* = nullptr);
            MapObject* contents(void) const;
            void empty(void);

        private:
            Terrain _terrain;
            MapObject* _contents;
    };
}

#endif
