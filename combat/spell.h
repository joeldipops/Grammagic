#ifndef SPELL_H_INCLUDED
#define SPELL_H_INCLUDED

#include "word.h"
#include "nounish.h"
#include "verb.h"
#include <vector>
#include "battleField.h"

class Mob;

class Spell
{
    public:
        Spell(){};
        Spell(Nounish*, Nounish*, Verb, std::vector<Word> = std::vector<Word>(0));

        int cast(Mob*, BattleField*);

    private:
        Nounish* _target;
        Nounish* _source;
        Verb _action;
        std::vector<Word> _adjectives;
};

#endif
