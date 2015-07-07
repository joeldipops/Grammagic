#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include "mob.h"
#include "gameMap.h"

class Enemy : public Mob
{
    public:
        Enemy(void);
        bool aiMove(GameMap*);
        void aiAct(BattleField*);
        int movementDelay(void) const;
        int combatDelay() ;

    private:
        int _movementDelay;
        int _combatDelay;
        int _physicalStrength;
        void attack(BattleField*);
};

#endif
