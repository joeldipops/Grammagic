#include "verb.h"

using namespace Magic;

/**
 * Constructor
 */
Verb::Verb(
    Action action_, std::string n, Modifier e, Modifier c, Modifier d,
    float enemyCostMultiplier_, float allyEffectMultiplier_, float isSameMultiplier_,
    bool isBoon_
)
    :Word(n, e, c, d)
{
    _action = action_;
    _enemyCostMultiplier = enemyCostMultiplier_;
    _allyEffectMultiplier = allyEffectMultiplier_;
    _isSameMultiplier = isSameMultiplier_;
    _isBoon = isBoon_;
}


WordType Verb::type(void) const
{
    return WordType::AVerb;
}

/**
 * Limits, increases or inverts the cost if the caster uses an enemy as the source of this spell.
 * eg. you can use enemies as sources of spells, but their will be no ill effects to them.
 */
float Verb::enemyCostMultiplier(void) const
{
    return _enemyCostMultiplier;
}

/**
 * Limits, increases or inverts the effect if the caster casts this spell on their ally.
 *...I can't remember the use case for this, if it exists...
 */
float Verb::allyEffectMultiplier(void) const
{
    return _allyEffectMultiplier;
}

/**
 * Limits, increases or inverts the effect if the target is the same as the source.
 * This prevents things like free healing or damage.
 */
float Verb::isSameMultiplier(void) const
{
    return _isSameMultiplier;
}

/**
 * If an spell is a boon, it will ignore magical resistance.
 */
bool Verb::isBoon(void) const
{
    return _isBoon;
}

/**
 *
 */
Action Verb::action(void)
{
    return _action;
}
