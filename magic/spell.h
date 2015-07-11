#ifndef SPELL_H_INCLUDED
#define SPELL_H_INCLUDED

#include <vector>

#include "word.h"
#include "nounish.h"
#include "verb.h"
#include "adverb.h"
#include "battleField.h"

class Mob;
namespace Magic
{
    class Spell
    {
        public:
            Spell(){};
            Spell(Nounish*, Nounish*, Verb, std::vector<Adverb> = std::vector<Adverb>(0));

            int cast(Mob*, BattleField*);

        private:
            Nounish* _target;
            Nounish* _source;
            Verb _action;
            std::vector<Word> _adjectives;
    };
}
#endif
