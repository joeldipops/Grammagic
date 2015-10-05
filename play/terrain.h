#ifndef TERRAIN_H_INCLUDED
#define TERRAIN_H_INCLUDED

#include "mapObject.h"
#include "../globalConstants.h"
#include "../res/templates.h"
#include "party.h"

namespace Play
{
    class Terrain : public MapObject
    {
        public:
            Terrain(void);
            Terrain(const Resources::TerrainTemplate&);
            PlayStateContainer& onInspect(PlayStateContainer&);
            PlayStateContainer& onEnter(PlayStateContainer&);
        private:
            PlayEventHandler _onEnter;
    };
}

#endif
