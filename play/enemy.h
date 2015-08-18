#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include "mob.h"
#include "gameMap.h"
#include "../globalConstants.h"
#include "../res/templates.h"

using namespace Magic;
namespace Play
{
    typedef int (*AiAction)(Mob* actor, BattleField* field);

    class Enemy : public Mob
    {
        public:
            Enemy(const Templates::EnemyTemplate&);
            bool aiMove(GameMap&);
            void aiAct(BattleField*);
            PlayStateContainer& onInspect(PlayStateContainer&);
            int movementDelay(void) const;
            int combatDelay(void) const;
            float physicalStrength(void) const;
            MobType type(void) const;

        private:
            int _movementDelay;
            int _combatDelay;
            int _physicalStrength;
            AiAction _combatAction;
            void attack(BattleField*);
    };
}

#endif
