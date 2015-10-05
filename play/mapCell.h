#ifndef MAPCELL_H_INCLUDED
#define MAPCELL_H_INCLUDED

#include "terrain.h"
#include "mob.h"

namespace Play
{
    class MapCell
    {
        public:
            MapCell(void) {};
            MapCell(const Resources::TerrainTemplate&);
            ~MapCell(void);
            Terrain* terrain(Terrain* = nullptr);
            const Terrain* terrain(void) const;
            PlayStateContainer* enter(MapObject*, PlayStateContainer*);
            MapObject* contents(MapObject*);
            MapObject* contents(void) const;
            PlayStateContainer& onInspect(PlayStateContainer&);
            void empty(void);

        private:
            Terrain _terrain;
            MapObject* _contents;
    };
}

#endif
