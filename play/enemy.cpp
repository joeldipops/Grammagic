#include "enemy.h"
#include "playStateManager.h"

using namespace Play;

Enemy::Enemy(const Templates::EnemyTemplate& tmpl) : Mob(tmpl, MobType::Hostile)
{
    _physicalStrength = tmpl.Attack;
    _combatDelay = tmpl.AttackDelay;
    _movementDelay = tmpl.MovementDelay;
    _combatAction = tmpl.CombatAction;
}

MobType Enemy::type(void) const
{
    return MobType::Hostile;
}

int Enemy::combatDelay(void) const
{
    return _combatDelay;
}

/**
 * Allow an NPC to move according to it's AI.
 * @param map The map the mob should move around.
 * @return true if the move was succesful.
 */
bool Enemy::aiMove(GameMap& map_)
{
    int dX = 0;
    int dY = 0;

    // Chase the party.
    if (isSensed((MapObject&)*map_.party()))
    {
        int tX = map_.party()->x();
        int tY = map_.party()->y();

        if (tX > x())
            dX = 1;
        else if (tX < x())
            dX = -1;

        if (tY > y())
            dY = 1;
        else if (tY < y())
            dY = -1;
    }
    // Or just move randomly
    else
    {
        dY = (rand() % 100) > 50 ? 1 : -1;
        dX = (rand() % 100) > 50 ? 1 : -1;
    }

    block(SDL_GetTicks() + _movementDelay + (rand() % 200 - 100));

    if (dX == 0 && dY == 0)
        return false;

    return map_.moveMob(this, x() + dX, y() + dY);
}

/**
 * Allow an NPC to attack according to tis AI
 * @param field The current battlefield.
 */
void Enemy::aiAct(BattleField* field)
{
    int dur = _combatAction(this, field);
    block(SDL_GetTicks() + (dur / speed()));
}

PlayStateContainer& Enemy::onInspect(PlayStateContainer& data)
{
    data.State = PlayState::Combat;
    isInCombat(true);
    return data;
}

int Enemy::movementDelay(void) const { return _movementDelay; }

float Enemy::physicalStrength(void) const { return _physicalStrength; }
