#ifndef SPELL_H_INCLUDED
#define SPELL_H_INCLUDED

#include <vector>
#include <math.h>

#include "word.h"
#include "verb.h"
#include "adverb.h"
#include "noun.h"
#include "../play/battleField.h"
#include "../util/utils.h"
#include "rune.h"
#include "command.h"

namespace Play { class Mob; }
namespace Magic
{
    class Spell : public Command
    {
        public:
            static bool verify(std::vector<Rune*>);
            bool isValid(void) const;
            bool isValid(bool) const;

            Spell(){};
            Spell(std::vector<Word*>);
            Spell(std::vector<Rune*> components_);
            ~Spell(void);

            const std::vector<Rune*> components(void) const;
            const Rune* component(natural) const;
            Rune* component(natural, Rune*);
            void removeComponent(natural);
            void addComponent(Rune*, bool = false);

            bool edit(std::vector<Rune*>);
            int execute(Mob*, SpellContext&);
            bool resolve(void);

        private:
            int calculateCost(void) const;
            int calculateEffect(void) const;
            int calculateDuration(void) const;

            Noun* _target;
            Noun* _source;
            Verb* _action;
            std::vector<Adverb*> _adverbs;
            void toBin(Word*);
            void emptyBin(void);
            std::vector<Word*> _rubbishBin;
            std::vector<Rune*> _components;
    };
}
#endif
