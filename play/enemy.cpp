#include "enemy.h"
#include <stdlib.h>

Enemy::Enemy(void) : Mob(MobType::Hostile)
{
    imageFileName(RESOURCE_LOCATION + "enemy.png");
    _physicalStrength = 10;
    _combatDelay = 2000;
    _movementDelay = 500;
}

int Enemy::combatDelay()
{
    return _combatDelay;
}

/**
 * Allow an NPC to move according to it's AI.
 * @param map The map the mob should move around.
 * @return true if the move was succesful.
 */
bool Enemy::aiMove(GameMap* map_)
{
    bool xR = (rand() % 100) > 50;
    bool yR = (rand() % 100) > 50;


    block(SDL_GetTicks() + _movementDelay + (rand() % 200 - 100));
    return map_->moveMob(
        this,
        xR ? x() + 1 : x() - 1,
        yR ? y() + 1 : y() - 1
    );
}

/**
 * Allow an NPC to attack according to tis AI
 * @param field The current battlefield.
 */
void Enemy::aiAct(BattleField* field)
{
    attack(field);
}

/**
 * Does a fixed amount of damage to some enemy mob.
 * @param field The battlefield.
 */
void Enemy::attack(BattleField* field)
{
    for (int i = 0; i < int(field->mobs().size()); i++)
    {
        if (!field->areAllied(this, field->mobs().at(i)))
        {
            field->mobs().at(i)->changeStamina(-1 * _physicalStrength);
            break;
        }
    }

    block(SDL_GetTicks() + _combatDelay);
}

int Enemy::movementDelay(void) const
{
    return _movementDelay;
}
