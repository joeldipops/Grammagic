#include "verb.h"
Verb::Verb(Action action_, std::string n, Modifier e, Modifier c, Modifier d, float enemyCostMultiplier_, float allyEffectMultiplier_, float isSameMultiplier_)
    :Word(n, e, c, d)
{
    _action = action_;
    _enemyCostMultiplier = enemyCostMultiplier_;
    _allyEffectMultiplier = allyEffectMultiplier_;
    _isSameMultiplier = isSameMultiplier_;
}

float Verb::enemyCostMultiplier(void) const
{
    return _enemyCostMultiplier;
}

float Verb::allyEffectMultiplier(void) const
{
    return _allyEffectMultiplier;
}

float Verb::isSameMultiplier(void) const
{
    return _isSameMultiplier;
}

Action Verb::action(void)
{
    return _action;
}
