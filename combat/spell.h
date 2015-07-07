#ifndef SPELL_H_INCLUDED
#define SPELL_H_INCLUDED

#include "word.h"
#include "noun.h"
#include "verb.h"
#include <vector>
#include "battleField.h"

class Mob;

class Spell
{
    public:
        Spell(){};
        Spell(Noun, Noun, Verb, std::vector<Word> = std::vector<Word>(0));
        int cast(Mob*, BattleField*);

    private:
        Noun _target;
        Noun _source;
        Verb _action;
        std::vector<Word> _adjectives;
};

#endif
