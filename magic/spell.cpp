#include "spell.h"

// Forward Reference
#include "../play/mob.h"

using namespace Magic;

DummyAdjective Spell::_dummy = DummyAdjective(Spell::_randomObj, "", Modifier(0, 1.0), Modifier(0, 1.0), Modifier(0, 1.0));


//{Static Functions
/**
 * Static function to determine if a list of words can be turned in to a valid spell.
 * @param A candidate list of spell components.
 * @return true if the list contains all the components to make a valid spell.
 */
bool Spell::verify_Deprecated(std::vector<Word*> components_Deprecated)
{
    bool hasTarget = false;
    bool hasSource = false;
    bool hasAction = false;

    for (natural i = 0; i < components_Deprecated.size(); i++)
    {
        switch(components_Deprecated.at(i)->type())
        {
            case WordType::NOUN:
            case WordType::NOUN_PHRASE:
                // Can't have more than one of each (yet)
                if (hasTarget && hasSource)
                    return false;
                if (hasSource)
                    hasTarget = true;
                else
                    hasSource = true;
                break;
            case WordType::ADJECTIVE:
                if (hasTarget && hasSource)
                    return false;
                // An adjective must be followed by a noun
                if (components_Deprecated.size() <= i+1)
                    return false;
                if (components_Deprecated.at(i+1)->type() != WordType::NOUN)
                    return false;
                if (hasSource)
                    hasTarget = true;
                else
                    hasSource = true;
                // We just did two components at once.
                i++;
                break;
            case WordType::VERB:
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
//}

//{LifeCycle
Spell::Spell(std::vector<Rune*> components_)
{
    _source = nullptr;
    _target = nullptr;
    _action = nullptr;
    _adverbs = std::vector<Adverb*>();
    if (!edit(components_))
    {
        _source = nullptr;
        _target = nullptr;
        _action = nullptr;
        _components = components_;
    }
}

/**
 * Constructor
 */
Spell::Spell(std::vector<Word*> components_)
{

    _source = nullptr;
    _target = nullptr;
    _action = nullptr;
    _adverbs = std::vector<Adverb*>();
    if(!edit_Deprecated(components_))
    {
        _source = nullptr;
        _target = nullptr;
        _action = nullptr;
        _components_Deprecated = components_;
    }
}

/**
 * Destructor
 */
Spell::~Spell(void)
{
    for (Word* w : _rubbishBin)
    {
        delete w;
        w = nullptr;
    }

    _rubbishBin = std::vector<Word*>(0);
}
//}

//{Properties

const std::vector<Rune*> Spell::components(void) const { return _components; }

/**
 * Gets a list of each word in the spell.
 */
const std::vector<Word*> Spell::components_Deprecated(void) const { return _components_Deprecated; }
//}

//{Methods
/**
 * Takes the current component list and attempts to turn it in to a spell.
 * @return true if spell was valid, false otherwise.
 */
bool Spell::resolve_Deprecated(void)
{
    _target = nullptr;
    _source = nullptr;
    _action = nullptr;
    _adverbs = std::vector<Adverb*>(0);
    return edit_Deprecated(_components_Deprecated);
}

/**
 * Keep track of
 */
void Spell::toBin(Word* word)
{
    _rubbishBin.push_back(word);
}

void Spell::emptyBin(void)
{
    for (Word* w : _rubbishBin)
    {
        delete w;
    }
}

/**
 * Repopulates the spell from a list of runes.
 */
bool Spell::edit(std::vector<Rune*> components_)
{
    std::vector<Word*> addresses = std::vector<Word*>();
    Noun* target = nullptr;
    Noun* source = nullptr;
    Verb* action = nullptr;
    std::vector<Adverb*> adverbs(0);
    for (natural i = 0; i < components_.size(); i++)
    {
        Rune* rune = components_.at(i);

        if (source == nullptr)
        {
            // First part of the spell needs to identify a Source.
            // Possible phrases are
            // [ProperNoun]
            // [Aux][Adj][Noun]
            // [Adj][Noun]
            if (rune->isProperNoun())
            {
                source = new Noun(rune);
                addresses.push_back(source);
                continue;
            }

            if (rune->isAuxilliary())
            {
                if (components_.at(i+1)->isAdjective() && components_.at(i+2)->isNoun())
                {
                    source = new Noun(rune, components_.at(i+1), components_.at(i+2));
                    addresses.push_back(source);
                    i += 2;
                    continue;
                }
            }

            if (rune->isAdjective())
            {
                if (components_.at(i+1)->isNoun())
                {
                    source = new Noun(rune, components_.at(i+1));
                    addresses.push_back(source);
                    i += 1;
                    continue;
                }
            }
        }

        if (action == nullptr)
        {
            // Second part of the spell in the action.
            // [Aux][Verb]
            // [Verb]
            if (rune->isAuxilliary())
            {
                if (components_.at(i+1)->isVerb())
                {
                    action = new Verb(rune, components_.at(i+1));
                    addresses.push_back(action);
                    i += 1;
                    continue;
                }
            }
            if (rune->isVerb())
            {
                action = new Verb(rune);
                addresses.push_back(action);
                continue;
            }
        }

        if (target == nullptr)
        {
            if (rune->isProperNoun())
            {
                target = new Noun(rune);
                addresses.push_back(target);
                continue;
            }

            if (rune->isAuxilliary())
            {
                if (components_.at(i+1)->isAdjective() && components_.at(i+2)->isNoun())
                {
                    target = new Noun(rune, components_.at(i+1), components_.at(i+2));
                    addresses.push_back(target);
                    i += 2;
                    continue;
                }
            }

            if (rune->isAdjective())
            {
                if (components_.at(i+1)->isNoun())
                {
                    target = new Noun(rune, components_.at(i+1));
                    addresses.push_back(target);
                    i += 1;
                    continue;
                }
            }
        }
        if (rune->isAdverb())
        {
            Adverb* adv = new Adverb(rune);
            adverbs.push_back(adv);
            addresses.push_back(adv);
        }
    }

    // If we are missing any of these, the spell is invalid.
    if (target == nullptr || action == nullptr || source == nullptr)
    {
        for(Word* w : addresses)
        {
            delete w;
        }
        return false;
    }

    _target = target;
    _action = action;
    _source = source;

    emptyBin();
    toBin(target);
    toBin(action);
    toBin(source);
    _adverbs = std::vector<Adverb*>();
    for (Adverb* w : adverbs)
    {
        toBin(w);
        _adverbs.push_back(w);
    }

    return true;
}

/**
 * Repopulates the spell from a list of words.
 * @param components The list of words.
 * @return true if the components were valid.
 */
bool Spell::edit_Deprecated(std::vector<Word*> components_)
{
    Nounish* target = nullptr;
    Nounish* source = nullptr;

    Verb* action = nullptr;
    std::vector<Adverb*> adverbs = std::vector<Adverb*>(0);
    for (natural i = 0; i < components_.size(); i++)
    {
        switch(components_.at(i)->type())
        {
            // If we encounter a noun without an adjective, use the dummy adjective.
            case WordType::NOUN: {
                if (source != nullptr && target != nullptr)
                    return false;

                NounPhrase* temp = new NounPhrase((Noun*)components_.at(i), &_dummy);
                // Keep track of allocated memory.
                toBin(temp);
                if (source != nullptr)
                    target = temp;
                else
                    source = temp;
                break;
            }
            case WordType::NOUN_PHRASE:
                // Can't have more than one of each (yet)
                if (target != nullptr && source != nullptr)
                    return false;
                if (source != nullptr)
                    target = (Nounish*) components_.at(i);
                else
                    source = (Nounish*) components_.at(i);
                break;
            case WordType::ADJECTIVE: {
                if (target != nullptr && source != nullptr)
                    return false;
                // An adjective must be followed by a noun
                if (components_.size() <= i + 1)
                    return false;
                if (components_.at(i+1)->type() != WordType::NOUN)
                    return false;

                NounPhrase* temp = new NounPhrase((Noun*)components_.at(i+1), (Adjective*)components_.at(i));
                // Keep track of allocated memory.
                toBin(temp);
                if (source != nullptr)
                    target = temp;
                else
                    source = temp;
                i++;
                break;
            }
            case WordType::VERB:
                // Can't have more than one verb (yet)
                if (action != nullptr)
                    return false;
                action = (Verb*) components_.at(i);
                break;
            case WordType::ADVERB:
                adverbs.push_back((Adverb*)components_.at(i));
                break;
            default:
                return false;
        }
    }

    if(target == nullptr || source == nullptr || action == nullptr)
        return false;

    if (_target_Deprecated != nullptr && _target_Deprecated->components_Deprecated().size() > 1)
        delete _target_Deprecated;

    if (_source_Deprecated != nullptr && _source_Deprecated->components_Deprecated().size() > 1)
        delete _source_Deprecated;

    _target_Deprecated = target;
    _source_Deprecated = source;

    // Clean up any noun-phrases we didn't use.
    for (Word* w : _rubbishBin)
        if (w == target || w == source)
            continue;
        else
        {
            delete w;
            w = nullptr;
        }
    _rubbishBin = std::vector<Word*>();

    _action = action;
    _adverbs.insert(_adverbs.end(), adverbs.begin(), adverbs.end());
    _components_Deprecated = components_;
    return true;

}

const Word* Spell::component(natural index) const
{
    return components_Deprecated().at(index);
}

Word* Spell::component(natural index, Word* word)
{
    if (_components_Deprecated.size() > index)
        _components_Deprecated.at(index) = word;
    else
        _components_Deprecated.push_back(word);
    resolve_Deprecated();
    return _components_Deprecated.at(index);
}

void Spell::addComponent(Word* word, bool doResolve)
{
    _components_Deprecated.push_back(word);
    if (doResolve)
        resolve_Deprecated();
}

void Spell::removeComponent(int index)
{
    int len = _components_Deprecated.size();
    if (len <= 0)
        return;
    if (index >= len)
        return;
    _components_Deprecated.erase(_components_Deprecated.begin() + index);
}

/**
 * @param checkUnresolved If true, also check unresolved components.
 * @return True if the spell can be effectively cast.
 */
bool Spell::isValid_Deprecated(bool checkUnresolved) const
{
    if (_target_Deprecated != nullptr
        && _source_Deprecated != nullptr
        && _action != nullptr
    )
        return true;

    // If checkComponents flag is provided, check the "Future" of the spell, not just the current state.eprec
    if (!checkUnresolved)
        return false;

    return verify_Deprecated(_components_Deprecated);
}

/**
 * Casts a spell on a target.
 * @param caster The mob who cast the spell.
 * @param battlefield cache of information about the current state of the battlefield.
 * @return The calculated duration of the spell
 */
int Spell::cast_Deprecated(Mob* caster, BattleField* battleField)
{
    if (!isValid_Deprecated())
            return -1;

    float totalEffect = _action->effect()->add(0);
    totalEffect = _target_Deprecated->effect()->add(totalEffect);
    totalEffect = _source_Deprecated->effect()->add(totalEffect);
    totalEffect = _action->effect()->multiply(totalEffect);
    totalEffect = _target_Deprecated->effect()->multiply(totalEffect);
    totalEffect = _source_Deprecated->effect()->multiply(totalEffect);

    // Apply caster power
    totalEffect *= caster->skill();


    float totalCost = _action->cost()->add(0);
    totalCost = _target_Deprecated->cost()->add(totalCost);
    totalCost = _source_Deprecated->cost()->add(totalCost);
    totalCost = _action->cost()->multiply(totalCost);
    totalCost = _source_Deprecated->cost()->multiply(totalCost);
    totalCost = _source_Deprecated->cost()->multiply(totalCost);

    float totalDuration = _action->duration()->add(0);
    totalDuration = _target_Deprecated->duration()->add(totalDuration);
    totalDuration = _source_Deprecated->duration()->add(totalDuration);
    totalDuration = _action->duration()->multiply(totalDuration);
    totalDuration = _target_Deprecated->duration()->multiply(totalDuration);
    totalDuration = _source_Deprecated->duration()->multiply(totalDuration);

    Adverb* word;
    for (natural i = 0; i < _adverbs.size(); i++)
    {
        word = _adverbs.at(i);

        totalEffect = word->effect()->modify(totalEffect);
        totalCost = word->cost()->modify(totalCost);
        totalDuration = word->duration()->modify(totalDuration);
    }

    Combatable* target = _target_Deprecated->acquireTarget(caster, battleField);
    Combatable* source = _source_Deprecated->acquireTarget(caster, battleField);

    // Prevent actions such as free healing where cost is 10 and effect is 30 = +20 health.
    if (target == source)
    {
        totalEffect *= _action->isSameMultiplier();
        totalCost *= _action->isSameMultiplier();
    }

    // Ensure there is a penalty for using enemy sources rather than allied ones.
    if (!battleField->areAllied((Combatable*)caster, source))
        totalCost *= _action->enemyCostMultiplier();

    if (battleField->areAllied((Combatable*)caster, target))
        totalEffect *= _action->allyEffectMultiplier();

    // Apply magical resistance.
    if (!_action->isBoon())
        totalEffect *= target->resistance();

    SpellData data;
    _action->action()(source, target, totalCost, totalEffect, data);

    return ceil(totalDuration / caster->speed());
}
//}
