#ifndef SPELL_H_INCLUDED
#define SPELL_H_INCLUDED

#include <vector>
#include <math.h>

#include "word.h"
#include "nounish.h"
#include "verb.h"
#include "adverb.h"
#include "nounPhrase.h"
#include "../play/battleField.h"
#include "../util/utils.h"
#include "rune.h"

namespace Play { class Mob; }
namespace Magic
{
    class Spell
    {
        public:
            static bool verify(std::vector<Rune*>);
            bool isValid(bool = false) const;

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
            int cast(Mob*, BattleField*);
            bool resolve(void);



            const Word* component_Deprecated(natural) const;
            Word* component_Deprecated(natural, Word*);
            void removeComponent_Deprecated(int);
            void addComponent_Deprecated(Word*, bool = false);
            static bool verify_Deprecated(std::vector<Word*>);
            bool isValid_Deprecated(bool = false) const;
            bool edit_Deprecated(std::vector<Word*>);
            bool resolve_Deprecated(void);
            int cast_Deprecated(Mob*, BattleField*);
            const std::vector<Word*> components_Deprecated(void) const;

        private:
            int calculateCost(void) const;
            int calculateEffect(void) const;
            int calculateDuration(void) const;

            Noun* _target;
            Noun* _source;
            Nounish* _target_Deprecated;
            Nounish* _source_Deprecated;
            Verb* _action;
            std::vector<Adverb*> _adverbs;
            void toBin(Word*);
            void emptyBin(void);
            std::vector<Word*> _rubbishBin;
            std::vector<Rune*> _components;


            std::vector<Word*> _components_Deprecated;

            static DummyAdjective _dummy;
            static Combatable* _randomObj (Mob* caster, BattleField* field, const std::vector<Combatable*>& candidates, SpellData&)
            {
                int index  = rand() % candidates.size();
                return candidates.at(index);
            }

    };
}
#endif
