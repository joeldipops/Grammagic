#include "spell.h"
using namespace Magic;

Spell::Spell(Nounish* source, Nounish* target, Verb action, std::vector<Adverb>* adverbs)
{
    _source = source;
    _target = target;
    _action = action;
    if (adverbs != nullptr)
        _adverbs = *adverbs;
    else
        _adverbs = std::vector<Adverb>();
}

/**
 * Gets a list of each word in the spell.
 */
const std::vector<Word*> Spell::components(void) const
{
        std::vector<Word*> result = std::vector<Word*>(0);
        result.reserve(5 + _adverbs.size());
        std::vector<Word*> subList = _source->components();
        result.insert(result.end(), subList.begin(), subList.end());

        for (int i = 0; i < int(_adverbs.size()); i++)
            result.push_back((Word*) &_adverbs.at(i));

        result.push_back((Word*) &_action);

        subList = _target->components();
        result.insert(result.end(), subList.begin(), subList.end());

        result.shrink_to_fit();
        return result;
}

const Word* Spell::component(int index) const
{
    return components().at(index);
}

/**
 * Replace one or more of the spell's components words.
 */
void Spell::edit(Nounish* source, Nounish* target, Verb* action)
{
    if (source != nullptr)
        _source = source;
    if (target != nullptr)
        _target = target;
    if (action != nullptr)
        _action = *action;
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

    Adverb word;
    for (int i = 0; i < int(_adverbs.size()); i++)
    {
        word = _adverbs.at(i);

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

