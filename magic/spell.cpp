#include "spell.h"

// Forward Reference
#include "../play/mob.h"

using namespace Magic;

//{Static Functions
/**
 * Static function to determine if a list of words can be turned in to a valid spell.
 * @param A candidate list of spell components.
 * @return true if the list contains all the components to make a valid spell.
 */

bool Spell::verify(std::vector<Rune*> components_)
{
    bool hasSource = false;
    bool hasAction = false;
    bool hasTarget = false;

    try { for (natural i = 0; i < components_.size(); i++)
    {
        Rune* rune = components_.at(i);

        if (!hasSource)
        {
            if (rune->isProperNoun())
            {
                // run in case an exception is called.
                Noun(rune);
                hasSource = true;
                continue;
            }

            if (rune->isAuxilliary())
            {
                if (components_.size() > i + 2 && components_.at(i+1)->isAdjective() && components_.at(i+2)->isNoun())
                {
                    Noun(rune, components_.at(i+1), components_.at(i+2));
                    hasSource = true;
                    i += 2;
                    continue;
                }
            }

            if (rune->isAdjective())
            {
                if (components_.size() > i + 1 && components_.at(i+1)->isNoun())
                {
                    Noun(rune, components_.at(i+1));
                    hasSource = true;
                    i += 1;
                    continue;
                }
            }
        }

        if (!hasAction)
        {
            // Second part of the spell in the action.
            // [Aux][Verb]
            // [Verb]
            if (rune->isAuxilliary())
            {
                if (components_.size() > i + 1 && components_.at(i+1)->isVerb())
                {
                    Verb(rune, components_.at(i+1));
                    hasAction = true;
                    i += 1;
                    continue;
                }
            }
            if (rune->isVerb())
            {
                Verb(rune);
                hasAction = true;
                continue;
            }
        }

        if (!hasTarget)
        {
            if (rune->isProperNoun())
            {
                Noun(rune);
                hasTarget = true;
                continue;
            }

            if (rune->isAuxilliary())
            {
                if (components_.size() > i + 2 && components_.at(i+1)->isAdjective() && components_.at(i+2)->isNoun())
                {
                    Noun(rune, components_.at(i+1), components_.at(i+2));
                    hasTarget = true;
                    i += 2;
                    continue;
                }
            }

            if (rune->isAdjective())
            {
                if (components_.size() > i + 1 && components_.at(i+1)->isNoun())
                {
                    Noun(rune, components_.at(i+1));
                    hasTarget = true;
                    i += 1;
                    continue;
                }
            }
        }
        if (!rune->isAdverb())
            return false;
    }} catch (...)
    {
        return false;
    }

    return hasTarget && hasAction && hasSource;
}
//}

//{LifeCycle

/**
 * Constructor
 */
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
 * Destructor
 */
Spell::~Spell(void)
{
    for (Word* w : _rubbishBin)
        deletePtr(w);


    _rubbishBin = std::vector<Word*>(0);
}
//}

//{ Properties

const std::vector<Rune*> Spell::components(void) const { return _components; }

//}

//{ Methods
bool Spell::resolve(void)
{
    _target = nullptr;
    _source = nullptr;
    _action = nullptr;
    _adverbs = std::vector<Adverb*>(0);
    return edit(_components);
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
        delete w;

    _rubbishBin = std::vector<Word*>();
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
    try { for (natural i = 0; i < components_.size(); i++)
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
                if (
                    components_.size() > i + 2 &&
                    components_.at(i+1)->isAdjective() && components_.at(i+2)->isNoun()
                )
                {
                    source = new Noun(rune, components_.at(i+1), components_.at(i+2));
                    addresses.push_back(source);
                    i += 2;
                    continue;
                }
            }

            if (rune->isAdjective())
            {
                if (components_.size() > i + 1 && components_.at(i+1)->isNoun())
                {
                    source = new Noun(rune, components_.at(i+1));
                    addresses.push_back(source);
                    i += 1;
                    continue;
                }
            }
            break;
        }

        if (action == nullptr)
        {
            // Second part of the spell in the action.
            // [Aux][Verb]
            // [Verb]
            if (rune->isAuxilliary())
            {
                if (components_.size() > i + 1 && components_.at(i+1)->isVerb())
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
            break;
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
                if (
                    components_.size() > i + 2 &&
                    components_.at(i+1)->isAdjective() && components_.at(i+2)->isNoun()
                )
                {
                    target = new Noun(rune, components_.at(i+1), components_.at(i+2));
                    addresses.push_back(target);
                    i += 2;
                    continue;
                }
            }

            if (rune->isAdjective())
            {
                if (components_.size() > i + 1 && components_.at(i+1)->isNoun())
                {
                    target = new Noun(rune, components_.at(i+1));
                    addresses.push_back(target);
                    i += 1;
                    continue;
                }
            }
            break;
        }
        if (rune->isAdverb())
        {
            Adverb* adv = new Adverb(rune);
            adverbs.push_back(adv);
            addresses.push_back(adv);
        }
    }} catch (...)
    {
        return false;
    }

    // If we are missing any of these, the spell is invalid.
    if (target == nullptr || action == nullptr || source == nullptr)
    {
        for(Word* w : addresses)
            delete w;

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

int Spell::calculateCost(void) const
{
    float result = 0;
    result += _action->addCost();
    result += _target->addCost();
    result += _source->addCost();

    for (Adverb* a : _adverbs)
        result += a->addCost();

    return (int) floor(result);
}

int Spell::calculateEffect(void) const
{
    float result = 0;
    result += _action->addEffect();
    result += _target->addEffect();
    result += _source->addEffect();

    for (Adverb* a : _adverbs)
        result += a->addCost();

    return (int) floor(result);
}

int Spell::calculateDuration(void) const
{
    float result = 0;
    result += _action->addDuration();
    result += _target->addDuration();
    result += _source->addDuration();

    for (Adverb* a : _adverbs)
        result += a->addCost();

    return (int) floor(result);
}

bool Spell::isValid(void) const
{
    return isValid(false);
}

/**
 * @param checkUnresolved If true, also check unresolved components.
 * @return True if the spell can be effectively cast.
 */
bool Spell::isValid(bool checkUnresolved) const
{
    if (_target != nullptr && _source != nullptr && _action != nullptr)
        return true;

    // If checkComponents flag is provided, check the "Future" of the spell, not just the current state.eprec
    if (!checkUnresolved)
        return false;

    return verify(_components);
}

int Spell::execute(Mob* caster, SpellContext& battleField)
{
    if (!isValid(false))
        return -1;

    SpellData data;

    data.cost = calculateCost();

    data.effect = calculateEffect();
    data.effect *= caster->skill();

    data.duration = calculateDuration();
    data.duration = ceil(data.duration / caster->speed());

    Combatable* source = _source->acquireTarget(caster, battleField);
    Combatable* target = _target->acquireTarget(caster, battleField);

    if (source == nullptr || target == nullptr)
        return 0;


    // Prevent actions such as free healing where cost is 10 and effect is 30 = +20 health.
    if (target == source)
    {
        data.effect *= _action->isSameMultiplier();
        data.cost *= _action->isSameMultiplier();
    }

    // Ensure there is a penalty for using enemy sources rather than allied ones.
    if (!battleField.areAllied((Combatable*)caster, source))
        data.cost *= _action->enemyCostMultiplier();

    if (battleField.areAllied((Combatable*)caster, target))
        data.effect *= _action->allyEffectMultiplier();

    // Apply meta actions.
    for (Adverb* word : _adverbs)
        word->modifySpell(data);

    // Only apply resistance if this is a harmful spell.
    if  (!_action->isBoon())
        data.effect *= target->resistance();

    // When calculations are complete, change to ints.
    data.effect = ceil(data.effect);
    data.cost = floor(data.cost);
    data.duration = floor(data.duration);

    _action->performAction(source, target, int(data.cost), int(data.effect), data);

    return data.duration;
}

const Rune* Spell::component(natural index) const
{
    return _components.at(index);
}

Rune* Spell::component(natural index, Rune* rune)
{
    if (_components.size() > index)
        _components.at(index) = rune;
    else
    {
        index = _components.size();
        _components.push_back(rune);
    }

    resolve();
    return _components.at(index);
}

void Spell::removeComponent(natural index)
{
    natural len = _components.size();
    if (len <= 0)
        return;
    if (index >= len)
        return;
    _components.erase(_components.begin() + index);
}

void Spell::addComponent(Rune* rune, bool doResolve)
{
    _components.push_back(rune);
    if (doResolve)
        resolve();
}
//}

