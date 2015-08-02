#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include "mob.h"
#include "gameMap.h"

using namespace Magic;

class Enemy : public Mob
{
    public:
        Enemy(void);
        bool aiMove(GameMap&);
        void aiAct(BattleField*);
        int movementDelay(void) const;
        int combatDelay() const;
        MobType type(void) const;

    private:
        int _movementDelay;
        int _combatDelay;
        int _physicalStrength;
        void attack(BattleField*);
};

#endif
