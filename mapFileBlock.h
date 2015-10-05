#ifndef MAPFILEBLOCK_H_INCLUDED
#define MAPFILEBLOCK_H_INCLUDED

#include "play/mob.h"
#include "play/terrain.h"

namespace Persistence
{
    struct MapFileBlock
    {
        public:
            static const int BYTES_PER_CELL = 2;
            static MapFileBlock generateTestCell(TerrainType terrain, MobType mob = MobType::None) {
                MapFileBlock cell;
                cell.terrainType = terrain;
                cell.mobType = mob;
                return cell;
            };
            TerrainType terrainType;
            MobType mobType;
    };
}
#endif
