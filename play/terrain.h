#ifndef TERRAIN_H_INCLUDED
#define TERRAIN_H_INCLUDED

#include "../mapObject.h"
#include "../globalConstants.h"
#include "party.h"

namespace Play
{
    class Terrain : public MapObject
    {
        public:
            Terrain(void);
            Terrain(TerrainType);
            TerrainType type(void) const;
            PlayStateContainer& onInspect(PlayStateContainer&);
            std::string onEnter(Party*);

        private:
            TerrainType _terrainType;
    };
}

#endif
