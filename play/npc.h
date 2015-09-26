#ifndef NPC_H_INCLUDED
#define NPC_H_INCLUDED

#include "mob.h"

namespace Templates { struct EnemyTemplate; }

namespace Play
{
    class GameMap;

    class NPC : public Mob
    {
        public:
            NPC(const Templates::EnemyTemplate&, MobType);
            NPC(const Templates::EnemyTemplate&);
            ~NPC(void);
            virtual bool aiMove(GameMap&);
        protected:
            int movementDelay(void) const;
        private:
            int _movementDelay;
    };
}

#endif
