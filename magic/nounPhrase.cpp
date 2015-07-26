#include "nounPhrase.h"
using namespace Magic;
NounPhrase::NounPhrase(Noun* noun, Adjective* adjective)
    :Nounish(
        "",
        Modifier(
            int(noun->effect()->add(0) + adjective->effect()->add(0)),
            noun->effect()->multiply(1) * adjective->effect()->multiply(1) * 1.0
        ),
        Modifier(
            int(noun->cost()->add(0) + adjective->cost()->add(0)),
            noun->cost()->multiply(1) * adjective->cost()->multiply(1) * 1.0
        ),
        Modifier(
            int(noun->duration()->add(0) + adjective->duration()->add(0)),
            noun->duration()->multiply(1) * adjective->duration()->multiply(1) * 1.0
        )
    )
{
    _targeter = noun;
    _specifier = adjective;
}

Combatable* NounPhrase::acquireTarget(Mob* caster, BattleField* field)
{
    std::vector<Combatable*> candidates = _targeter->acquireCandidates(caster, field);
    return _specifier->selectTarget(caster, field, candidates);
}

WordType NounPhrase::type(void) const
{
    return WordType::ANounPhrase;
}

const std::vector<Word*> NounPhrase::components(void) const
{
    std::vector<Word*> result  = std::vector<Word*> (0);
    result.reserve(2);
    if (!_specifier->isDummy())
        result.push_back((Word*) _specifier);
    if (!_targeter->isDummy())
        result.push_back((Word*) _targeter);
    return result;
}
