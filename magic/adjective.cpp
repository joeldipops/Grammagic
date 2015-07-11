#include "adjective.h"

using namespace Magic;

Adjective::Adjective(Selecter selecter, std::string n, Modifier e, Modifier c, Modifier d)
    : Word(n, e, c, d)
{
    _selecter = selecter;
}

/*
WordType Adjective::type(void) const
{
    return WordType::Adjective;
}*/

MapObject* Adjective::selectTarget(Mob* caster, BattleField* field, std::vector<MapObject*> candidates)
{
    return _selecter(caster, field, candidates);
}
