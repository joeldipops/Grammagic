#ifndef SPELL_H_INCLUDED
#define SPELL_H_INCLUDED

#include <vector>

#include "word.h"
#include "nounish.h"
#include "verb.h"
#include "adverb.h"
#include "nounPhrase.h"
#include "../play/battleField.h"
#include "../util/utils.h"

namespace Play { class Mob; }
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
            const Word* component(natural) const;
            Word* component(natural, Word*);
            void removeComponent(int);
            void addComponent(Word*, bool = false);
            bool isValid(bool = false) const;

        private:
            Nounish* _target;
            Nounish* _source;
            Verb* _action;
            std::vector<Adverb*> _adverbs;
            void toBin(Word*);
            std::vector<Word*> _rubbishBin;
            std::vector<Word*> _components;

            static DummyAdjective _dummy;
            static Combatable* _randomObj (Mob* caster, BattleField* field, std::vector<Combatable*> candidates)
            {
                int index  = rand() % candidates.size();
                return candidates.at(index);
            }

    };
}
#endif
