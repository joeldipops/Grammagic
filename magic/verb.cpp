#include "verb.h"

using namespace Magic;

//{Friends
void Magic::verbAct(const Verb* context, Combatable* source, Combatable* target, int cost, int effect, SpellData& data)
{
    return context->_action(source, target, cost, effect, data);
}

void Magic::auxVerbAct(const Verb* context, Combatable* source, Combatable* target, int cost, int effect, SpellData& data)
{
    data.modality = context->_modality;
    return context->_action(source, target, cost, effect, data);
}
//}

//{Lifecycle
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

Verb::Verb(Rune* action)
    : Word(std::vector<Rune*> {action})
{
    if (!action->isVerb())
        throw;

    _actionCostBonus = action->actionCostBonus();
    _actionDurationBonus = action->actionDurationBonus();
    _actionEffectBonus = action->actionEffectBonus();
    _isSameMultiplier = action->isSameMultiplier();
    _enemyCostMultiplier = action->enemyCostMultiplier();
    _allyEffectMultiplier = action->allyEffectMultiplier();
    _actionWrapper = verbAct;
};

Verb::Verb(Rune* aux, Rune* action)
    : Word(std::vector<Rune*> {aux, action})
{
    if (!aux->isAuxilliary() || !action->isVerb())
        throw;

    _actionWrapper = auxVerbAct;
};
//}

//{Properties
/**
 * Limits, increases or inverts the cost if the caster uses an enemy as the source of this spell.
 * eg. you can use enemies as sources of spells, but their will be no ill effects to them.
 */
float Verb::enemyCostMultiplier(void) const { return _enemyCostMultiplier; }

/**
 * Limits, increases or inverts the effect if the caster casts this spell on their ally.
 *...I can't remember the use case for this, if it exists...
 */
float Verb::allyEffectMultiplier(void) const { return _allyEffectMultiplier; }

/**
 * Limits, increases or inverts the effect if the target is the same as the source.
 * This prevents things like free healing or damage.
 */
float Verb::isSameMultiplier(void) const { return _isSameMultiplier; }

/**
 * Function that will be called when the spell is cast.
 */
Action Verb::action(void) { return _action; }

int Verb::addDuration(void) const { return _actionDurationBonus + Word::addDuration(); }

int Verb::addCost(void) const { return _actionCostBonus + Word::addCost(); }

int Verb::addEffect(void) const { return _actionEffectBonus + Word::addEffect(); }

//}

//{ Methods
/**
 * If an spell is a boon, it will ignore magical resistance.
 */
bool Verb::isBoon(void) const
{
    if (_modality == Modality::LOW)
        return !_isBoon;
    else
        return _isBoon;
}

WordType Verb::type(void) const
{
    return WordType::VERB;
}

void Verb::performAction(Combatable* source, Combatable* target, int cost, int effect, SpellData& data) const
{
    return _actionWrapper(this, source, target, cost, effect, data);
}

//}
