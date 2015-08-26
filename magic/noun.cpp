#include "noun.h"

using namespace Magic;

Noun::Noun(MultiTargeter targeter, std::string n, Modifier e, Modifier c, Modifier d)
    : Word(n, e, c, d)
{
    _targeter = targeter;
}

Noun::Noun(Rune* properNoun){};
Noun::Noun(Rune* aux, Rune* adj, Rune* noun){};
Noun::Noun(Rune* adj, Rune* noun){};


WordType Noun::type(void) const
{
    return WordType::NOUN;
}


std::vector<Combatable*> Noun::acquireCandidates(Mob* caster, BattleField* field)
{
    SpellData data;
    return _targeter(caster, field, data);
}
