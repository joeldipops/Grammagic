#include "spell.h"
using namespace Magic;

Spell::Spell(Nounish* source, Nounish* target, Verb action, std::vector<Adverb> adverbs)
{
    _source = source;
    _target = target;
    _action = action;
}

/**
 *
 *
 *
 * @return The calculated duration of the spell
 */
int Spell::cast(Mob* caster, BattleField* battleField)
{
    int totalEffect = _action.effect()->add(0);
    totalEffect = _target->effect()->add(totalEffect);
    totalEffect = _source->effect()->add(totalEffect);
    totalEffect = _action.effect()->multiply(totalEffect);
    totalEffect = _target->effect()->multiply(totalEffect);
    totalEffect = _source->effect()->multiply(totalEffect);

    int totalCost = _action.cost()->add(0);
    totalCost = _target->cost()->add(totalCost);
    totalCost = _target->cost()->add(totalCost);
    totalCost = _action.cost()->multiply(totalCost);
    totalCost = _target->cost()->multiply(totalCost);
    totalCost = _source->cost()->multiply(totalCost);

    int totalDuration = _action.duration()->add(0);
    totalDuration = _target->duration()->add(totalDuration);
    totalDuration = _target->duration()->add(totalDuration);
    totalDuration = _action.duration()->multiply(totalDuration);
    totalDuration = _target->duration()->multiply(totalDuration);
    totalDuration = _source->duration()->multiply(totalDuration);

    Word word;
    for (int i = 0; i < int(_adjectives.size()); i++)
    {
        word = _adjectives.at(i);

        totalEffect = word.effect()->modify(totalEffect);
        totalCost = word.cost()->modify(totalCost);
        totalDuration = word.duration()->modify(totalDuration);
    }

    MapObject* target = _target->acquireTarget(caster, battleField);
    MapObject* source = _source->acquireTarget(caster, battleField);

    // Prevent actions such as free healing where cost is 10 and effect is 30 = +20 health.
    if (target == source)
    {
        totalEffect *= _action.isSameMultiplier();
        totalCost *= _action.isSameMultiplier();
    }

    // Ensure there is a penalty for using enemy sources rather than allied ones.
    if (!battleField->areAllied((MapObject*)caster, source))
        totalCost *= _action.enemyCostMultiplier();

    if (battleField->areAllied((MapObject*)caster, target))
        totalEffect *= _action.allyEffectMultiplier();

    _action.action()(source, target, totalCost, totalEffect);

    return totalDuration;
}

