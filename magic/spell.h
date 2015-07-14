#ifndef SPELL_H_INCLUDED
#define SPELL_H_INCLUDED

#include <vector>

#include "word.h"
#include "nounish.h"
#include "verb.h"
#include "adverb.h"
#include "nounPhrase.h"
#include "battleField.h"

#include "../util/utils.h"

class Mob;
namespace Magic
{
    class Spell
    {
        public:
            static bool verify(std::vector<Word*>);

            Spell(){};
            Spell(std::vector<Word*>);
            ~Spell(void);
            bool edit(std::vector<Word*>);
            bool resolve(void);
            int cast(Mob*, BattleField*);
            const std::vector<Word*> components(void) const;
            const Word* component(int) const;
            Word* component(int, Word*);
            bool isValid(bool = false) const;

        private:
            Nounish* _target;
            Nounish* _source;
            Verb* _action;
            std::vector<Adverb*> _adverbs;
            //void takeOutBin(void);
            std::vector<NounPhrase*> _rubbishBin;
            std::vector<Word*> _components;

    };
}
#endif
