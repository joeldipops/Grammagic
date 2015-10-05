#include "rune.h"
#include "../res/templates.h"
using namespace Magic;

//{ Lifecycle
Rune::Rune(const Resources::RuneTemplate& tmpl)
    :MenuItem(tmpl.Name, tmpl.ImagePath)
{
    _getTarget = tmpl.GetTarget;
    _getTargetCandidates = tmpl.GetTargetCandidates;
    _selectTargetFromCandidates = tmpl.SelectTargetFromCandidates;
    _performAction = tmpl.PerformAction;
    _metaAction = tmpl.MetaAction;
    _modality = tmpl.Flag;
    _addEffect = tmpl.AddEffect;
    _addCost = tmpl.AddCost;
    _addDuration = tmpl.AddDuration;
    _modifyEffect = tmpl.ModEffect;
    _modifyCost = tmpl.ModCost;
    _modifyDuration = tmpl.ModDuration;
    _isSameMultiplier = tmpl.IsSameMultiplier;
    _enemyCostMultiplier = tmpl.EnemyCostMultiplier;
    _allyEffectMultiplier = tmpl.AllyEffectMultiplier;
    _actionCostBonus = tmpl.ActionCostBonus;
    _actionDurationBonus = tmpl.ActionDurationBonus;
    _actionEffectBonus = tmpl.ActionEffectBonus;
    _isBoon = tmpl.IsBoon;
    _code = tmpl.Code;
}

Rune::~Rune(void) {}

//}

//{Properties

/**
 * If rune is used as a proper noun, returns a function that targets a specific mob.
 */
Targeter Rune::getTargetFn(void) const { return _getTarget; }

/**
 * If rune is used as a noun, returns a function that finds a group of mobs to target.
 */
MultiTargeter Rune::getTargetCandidatesFn(void) const { return _getTargetCandidates; }

/**
 * If rune was used as an adjective, returns a function that targets a specific mob from a group of candidates.
 */
Selecter Rune::selectTargetFromCandidatesFn(void) const { return _selectTargetFromCandidates; }

/**
 * If rune was used as a verb, returns a function that carries out an action.
 */
Action Rune::performActionFn(void) const { return _performAction; }

/**
 * If rune was used as an adverb, returns a function that carries out an action on the spell itself.
 */
MetaAction Rune::metaActionFn(void) const { return _metaAction; }

/**
 * If rune was used as an auxilliary, returns the modality of the spell.
 */
Modality Rune::modality(void) const { return _modality; }

/**
 * The amount this rune adds or subtracts from the effect of the spell.
 */
int Rune::addEffect(void) const { return _addEffect; }

/**
 * The amount this rune adds or subtracts from the cost of the spell.
 */
int Rune::addCost(void) const { return _addCost; }

/**
 * The amount this rune adds or subtracts from the duration of the spell.
 */
int Rune::addDuration(void) const { return _addDuration; }

/**
 * The modifier this rune multiplies the effect of the spell by.
 */
float Rune::modEffect(void) const { return _modifyEffect; }

/**
 * The modifier this rune multiplies the cost of the spell by.
 */
float Rune::modCost(void) const { return _modifyCost; }

/**
 * The modifier this rune multiplies the duration of the spell by.
 */
float Rune::modDuration(void) const { return _modifyDuration; }

float Rune::enemyCostMultiplier(void) const { return _enemyCostMultiplier; }
float Rune::allyEffectMultiplier(void) const { return _allyEffectMultiplier; }
float Rune::isSameMultiplier(void) const { return _isSameMultiplier; }
int Rune::actionEffectBonus(void) const { return _actionEffectBonus; }
int Rune::actionCostBonus(void) const { return _actionCostBonus; }
int Rune::actionDurationBonus(void) const { return _actionDurationBonus; }
bool Rune::isBoon(void) const { return _isBoon; }
Persistence::SavedObjectCode Rune::code(void) const { return _code; }

//}

//{Methods

/**
 * Rune can be used as a proper noun (ie. Targets a specific mob)
 */
bool Rune::isProperNoun(void) const
{
    return _getTarget != nullptr;
}

/**
 * Rune can be used to select a group of candidate targets.
 */
bool Rune::isNoun(void) const
{
    return _getTargetCandidates != nullptr;
}

/**
 * Rune can be used to elect one target from a group of candidates.
 */
bool Rune::isAdjective(void) const
{
    return _selectTargetFromCandidates != nullptr;
}

/**
 * Rune be used as the primary effect of the spell.
 */
bool Rune::isVerb(void) const
{
    return _performAction != nullptr;
}

/**
 * Rune can be used to modify the spell itself.
 */
bool Rune::isAdverb(void) const
{
    return _metaAction != nullptr;
}

/**
 * Rune can be used to change the effect of a part of the spell eg. refer to high or low speed when used with a speed rune.
 */
bool Rune::isAuxilliary(void) const
{
    return _modality != Modality::NA;
}

//}
