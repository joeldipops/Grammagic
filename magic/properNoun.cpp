#include "properNoun.h"

using namespace Magic;

ProperNoun::ProperNoun(Targeter target, std::string name, Modifier effect, Modifier cost, Modifier duration)
    : Nounish(name, effect, cost, duration)
{
    _target = target;
}


/**
 * Selects the target from a mob, item or terrain on the battlefield according to the _target function.
 * @param caster The mob casting the spell.
 * @param battlefield Controls general aspects of the current battle and holds references to all combatants.
 * @return an object to be involved in the spell.
 */
MapObject* ProperNoun::acquireTarget(Mob* caster, BattleField* battleField)
{
    return _target(caster, battleField);
}

WordType ProperNoun::type(void) const
{
    return WordType::ANoun;
}

const std::vector<Word*> ProperNoun::components(void) const
{
    return std::vector<Word*> {(Word*)this};
}