#include "spell.h"
using namespace Magic;

Spell::Spell(std::vector<Word*> components)
{

    _source = nullptr;
    _target = nullptr;
    _action = nullptr;
    _adverbs = std::vector<Adverb*>();
    if(!edit(components))
    {
        _source = nullptr;
        _target = nullptr;
        _action = nullptr;
    }
}

Spell::~Spell(void)
{
}

/**
 * Repopulates the spell from a list of words.
 * @param components The list of words.
 * @return true if the components were valid.
 */
bool Spell::edit(std::vector<Word*> components)
{
    Nounish* target = nullptr;
    Nounish* source = nullptr;
    Verb* action = nullptr;
    std::vector<Adverb*> adverbs = std::vector<Adverb*>(0);
    std::vector<NounPhrase*> toBin = std::vector<NounPhrase*>(0);
    for (int i = 0 ; i < int(components.size()); i++)
    {
        switch(components.at(i)->type())
        {
            // Should never encounter a noun without an adjective.
            case WordType::ANoun:
                return false;
            case WordType::ANounPhrase:
                // Can't have more than one of each (yet)
                if (target != nullptr && source != nullptr)
                    return false;
                if (source != nullptr)
                    target = (Nounish*) components.at(i);
                else
                    source = (Nounish*) components.at(i);
                break;
            case WordType::AnAdjective: {
                if (target != nullptr && source != nullptr)
                    return false;
                // An adjective must be followed by a noun
                if (components.at(i+1)->type() != WordType::ANoun)
                    return false;
                NounPhrase* temp = new NounPhrase((Noun*)components.at(i+1), (Adjective*)components.at(i));
                //toBin.push_back(temp);
                if (source != nullptr)
                    target = temp;
                else
                    source = temp;
                i++;
                break;
            }
            case WordType::AVerb:
                // Can't have more than one verb (yet)
                if (action != nullptr)
                    return false;
                action = (Verb*) components.at(i);
                break;
            case WordType::AnAdverb:
                adverbs.push_back((Adverb*)components.at(i));
                break;
            default:
                return false;
        }
    }

    if(target == nullptr || source == nullptr || action == nullptr)
        return false;

    //takeOutBin();
    // keep track of the "new" references so we can clean them up later...
    //_rubbishBin = toBin;
    _target = target;
    _source = source;
    _action = action;
    _adverbs = adverbs;
    return true;

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

        result.push_back((Word*) _action);

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
 * @param A candidate list of spell components.
 * @return true if the list contains all the components to make a valid spell.
 */
bool Spell::verify(std::vector<Word*> components) const
{
    bool hasTarget = false;
    bool hasSource = false;
    bool hasAction = false;

    for (int i = 0 ; i < int(components.size()); i++)
    {
        switch(components.at(i)->type())
        {
            // Should never encounter a noun without an adjective.
            case WordType::ANoun:
                return false;
            case WordType::ANounPhrase:
                // Can't have more than one of each (yet)
                if (hasTarget && hasSource)
                    return false;
                if (hasSource)
                    hasTarget = true;
                else
                    hasSource = true;
                break;
            case WordType::AnAdjective:
                if (hasTarget && hasSource)
                    return false;
                // An adjective must be followed by a noun
                if (components.at(i+1)->type() != WordType::ANoun)
                    return false;
                if (hasSource)
                    hasTarget = true;
                else
                    hasSource = true;
                // We just did two components at once.
                i++;
                break;
            case WordType::AVerb:
                // Can't have more than one verb (yet)
                if (hasAction)
                    return false;
                hasAction = true;
                break;
            default:
                // Can have as many adverbs as we like.
                break;
        }
    }

    return hasTarget && hasSource && hasAction;
}


/**
 * @return True if the spell can be effectively cast.
 */
bool Spell::isValid(void) const
{
    return _target != nullptr
    && _source != nullptr
    && _action != nullptr;
}

/**
 * Casts a spell on a target.
 * @param caster The mob who cast the spell.
 * @param battlefield cache of information about the current state of the battlefield.
 * @return The calculated duration of the spell
 */
int Spell::cast(Mob* caster, BattleField* battleField)
{
    if (!isValid())
        return -1;

    int totalEffect = _action->effect()->add(0);
    totalEffect = _target->effect()->add(totalEffect);
    totalEffect = _source->effect()->add(totalEffect);
    totalEffect = _action->effect()->multiply(totalEffect);
    totalEffect = _target->effect()->multiply(totalEffect);
    totalEffect = _source->effect()->multiply(totalEffect);

    int totalCost = _action->cost()->add(0);
    totalCost = _target->cost()->add(totalCost);
    totalCost = _target->cost()->add(totalCost);
    totalCost = _action->cost()->multiply(totalCost);
    totalCost = _target->cost()->multiply(totalCost);
    totalCost = _source->cost()->multiply(totalCost);

    int totalDuration = _action->duration()->add(0);
    totalDuration = _target->duration()->add(totalDuration);
    totalDuration = _target->duration()->add(totalDuration);
    totalDuration = _action->duration()->multiply(totalDuration);
    totalDuration = _target->duration()->multiply(totalDuration);
    totalDuration = _source->duration()->multiply(totalDuration);

    Adverb* word;
    for (int i = 0; i < int(_adverbs.size()); i++)
    {
        word = _adverbs.at(i);

        totalEffect = word->effect()->modify(totalEffect);
        totalCost = word->cost()->modify(totalCost);
        totalDuration = word->duration()->modify(totalDuration);
    }

    MapObject* target = _target->acquireTarget(caster, battleField);
    MapObject* source = _source->acquireTarget(caster, battleField);

    // Prevent actions such as free healing where cost is 10 and effect is 30 = +20 health.
    if (target == source)
    {
        totalEffect *= _action->isSameMultiplier();
        totalCost *= _action->isSameMultiplier();
    }

    // Ensure there is a penalty for using enemy sources rather than allied ones.
    if (!battleField->areAllied((MapObject*)caster, source))
        totalCost *= _action->enemyCostMultiplier();

    if (battleField->areAllied((MapObject*)caster, target))
        totalEffect *= _action->allyEffectMultiplier();

    _action->action()(source, target, totalCost, totalEffect);

    return totalDuration;
}

