#include "nounPhrase.h"
using namespace Magic;
NounPhrase::NounPhrase(Noun noun, Adjective adjective)
    :Nounish(
        "",
        Modifier(
            noun.effect()->add(0) + adjective.effect()->add(0),
            noun.effect()->multiply(1) * adjective.effect()->multiply(1) * 1.0
        ),
        Modifier(
            noun.cost()->add(0) + adjective.cost()->add(0),
            noun.cost()->multiply(1) * adjective.cost()->multiply(1) * 1.0
        ),
        Modifier(
            noun.duration()->add(0) + adjective.duration()->add(0),
            noun.duration()->multiply(1) * adjective.duration()->multiply(1) * 1.0
        )
    )
{
    _targeter = noun;
    _specifier = adjective;
}

MapObject* NounPhrase::acquireTarget(Mob* caster, BattleField* field)
{
    std::vector<MapObject*> candidates = _targeter.acquireCandidates(caster, field);
    return _specifier.selectTarget(caster, field, candidates);
}

WordType NounPhrase::type(void) const
{
    return WordType::ANounPhrase;
}

const std::vector<Word*> NounPhrase::components(void) const
{
    std::vector<Word*> result  = std::vector<Word*> (0);
    result.reserve(2);
    result.push_back((Word*) &_targeter);
    result.push_back((Word*) &_specifier);
    return result;
}