#include "word.h"

Word::Word(std::string name, Modifier effect, Modifier cost, Modifier duration)
{
    _name = name;
    _effect = effect;
    _cost = cost;
    _duration = duration;
}

const std::string Word::name(void) const
{
    return _name;
}


const Modifier* Word::effect(void) const
{
    return &_effect;
}
Modifier Word::effect(Modifier effect_)
{
    _effect = effect_;
    return _effect;
}

const Modifier* Word::cost(void) const
{
    return &_cost;
}
Modifier Word::cost(Modifier cost_)
{
    _cost = cost_;
    return _cost;
}

const Modifier* Word::duration(void) const
{
    return &_duration;
}
Modifier Word::duration(Modifier duration_)
{
    _duration = duration_;
    return _duration;
}
