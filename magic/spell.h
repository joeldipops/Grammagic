#ifndef SPELL_H_INCLUDED
#define SPELL_H_INCLUDED

#include <vector>

#include "word.h"
#include "nounish.h"
#include "verb.h"
#include "adverb.h"
#include "battleField.h"

#include "../util/utils.h"

class Mob;
namespace Magic
{
    class Spell
    {
        public:
            Spell(){};
            Spell(Nounish*, Nounish*, Verb, std::vector<Adverb>* = nullptr);

            int cast(Mob*, BattleField*);
            const std::vector<Word*> components(void) const;

        private:
            Nounish* _target;
            Nounish* _source;
            Verb _action;
            std::vector<Adverb> _adverbs;
    };
}
#endif
