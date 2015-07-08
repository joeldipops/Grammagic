#include "noun.h"

Noun::Noun(MultiTargeter targeter, std::string n, Modifier e, Modifier c, Modifier d)
    : Word(n, e, c, d)
{
    _targeter = targeter;
}


std::vector<MapObject*> Noun::acquireCandidates(Mob* caster, BattleField* field)
{
    return _targeter(caster, field);
}
