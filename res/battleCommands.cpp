#include "battleCommands.h"
#include "../play/enemy.h"

using namespace Resources;

// Non Spells
//{
int Commands::FLEE(Command*, Mob*, SpellContext& field)
{
    field.isInCombat(false);
    return 0;
};

int Commands::ATTACK(Command* command, Mob* actor, SpellContext& field)
{
    // PCs are squishy and can not attack physically
    if (actor->isPlayerControlled())
        return 0;

    Enemy* nmeActor = (Enemy*)actor;

    const int ATTACK_DURATION = 1000;
    // A limit to stop us looping forever if we are implausibly unlucky.
    const int randFail = 10;
    natural iter = 0;
    while(iter < randFail)
    {
        int i = rand() % field.participants().size();
        Combatable* target = field.participants().at(i);

        // Do damage to the target and then ollie outie.
        if (!field.areAllied(actor, target))
        {
            target->changeStamina(-1 * nmeActor->physicalStrength() * target->defence());
            return ATTACK_DURATION;
        }

        iter++;
    }
    return 0;
}
//}

