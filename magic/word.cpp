#include "word.h"

using namespace Magic;

//{Lifecycle
/**
 * Constructor
 */
Word::Word(std::vector<Rune*> data)
{
    for (Rune* r : data)
    {
        _addCost += r->addCost();
        _addDuration += r->addDuration();
        _addEffect += r->addEffect();

        _modCost *= r->modCost();
        _modDuration *= r->modDuration();
        _modEffect *= r->modEffect();
    }
}

Word::Word(std::string name, Modifier effect, Modifier cost, Modifier duration)
{
    _name = name;
    _effect = effect;
    _cost = cost;
    _duration = duration;
}

/**
 * Destructor
 */
Word::~Word(void) {}
//}

//{Properties
int Word::addEffect(void) const { return _addEffect; }
int Word::addCost(void) const { return _addCost; }
int Word::addDuration(void) const { return _addDuration; }
float Word::modEffect(void) const { return _modEffect; }
float Word::modCost(void) const { return _modCost; }
float Word::modDuration(void) const { return _modDuration; }

// ---- Deprecated

const std::string& Word::name(void) const { return _name; }
const Modifier* Word::effect(void) const { return &_effect; }
Modifier Word::effect(Modifier effect_)
{
    _effect = effect_;
    return _effect;
}

const Modifier* Word::cost(void) const { return &_cost; }
Modifier Word::cost(Modifier cost_)
{
    _cost = cost_;
    return _cost;
}

const Modifier* Word::duration(void) const { return &_duration; }
Modifier Word::duration(Modifier duration_)
{
    _duration = duration_;
    return _duration;
}
//}

bool Word::isDummy(void) const
{
    return false;
}
