#include "properNoun.h"

using namespace Magic;

//{Lifecycle
/**
 * Constructor
 */
ProperNoun::ProperNoun(Targeter target, std::string name, Modifier effect, Modifier cost, Modifier duration)
    : Nounish(name, effect, cost, duration)
{
    _target = target;
}
//}

//{Methods
/**
 * Selects the target from a mob, item or terrain on the battlefield according to the _target function.
 * @param caster The mob casting the spell.
 * @param battlefield Controls general aspects of the current battle and holds references to all combatants.
 * @return an object to be involved in the spell.
 */
Combatable* ProperNoun::acquireTarget(Mob* caster, BattleField* battleField)
{
    SpellData data;
    return _target(caster, battleField, data);
}

WordType ProperNoun::type(void) const
{
    return WordType::NOUN_PHRASE;
}

const std::vector<Word*> ProperNoun::components_Deprecated(void) const
{
    return std::vector<Word*> {(Word*)this};
}
//}
