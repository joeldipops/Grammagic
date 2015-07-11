#include "noun.h"
using namespace Magic;

Noun::Noun(MultiTargeter targeter, std::string n, Modifier e, Modifier c, Modifier d)
    : Word(n, e, c, d)
{
    _targeter = targeter;
}

/*
WordType Noun::type(void) const
{
    return WordType::Noun;
}*/


std::vector<MapObject*> Noun::acquireCandidates(Mob* caster, BattleField* field)
{
    return _targeter(caster, field);
}
