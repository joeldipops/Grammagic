#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include "mob.h"
#include "gameMap.h"
#include "../globalConstants.h"
#include "../res/templates.h"
#include "npc.h"

using namespace Magic;
namespace Play
{
    typedef int (*AiAction)(Mob* context, SpellContext& field);

    class Enemy : public NPC
    {
        public:
            Enemy(const Resources::EnemyTemplate&);
            bool aiMove(GameMap&);
            void aiAct(BattleField&);
            PlayStateContainer& onInspect(PlayStateContainer&);
            int combatDelay(void) const;
            float physicalStrength(void) const;
            MobType type(void) const;
            int rewardForDefeat(void) const;

        private:
            int _combatDelay;
            int _physicalStrength;
            int _rewardForDefeat;
            AiAction _combatAction;
            void attack(BattleField&);
    };
}

#endif
