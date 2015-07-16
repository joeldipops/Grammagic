#include "verb.h"

using namespace Magic;

Verb::Verb(Action action_, std::string n, Modifier e, Modifier c, Modifier d, double enemyCostMultiplier_, double allyEffectMultiplier_, double isSameMultiplier_)
    :Word(n, e, c, d)
{
    _action = action_;
    _enemyCostMultiplier = enemyCostMultiplier_;
    _allyEffectMultiplier = allyEffectMultiplier_;
    _isSameMultiplier = isSameMultiplier_;
}


WordType Verb::type(void) const
{
    return WordType::AVerb;
}

double Verb::enemyCostMultiplier(void) const
{
    return _enemyCostMultiplier;
}

double Verb::allyEffectMultiplier(void) const
{
    return _allyEffectMultiplier;
}

double Verb::isSameMultiplier(void) const
{
    return _isSameMultiplier;
}

Action Verb::action(void)
{
    return _action;
}
