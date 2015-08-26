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
#include "rune.h"

namespace Play { class Mob; }
namespace Magic
{
    class Spell
    {
        public:
            static bool verify_Deprecated(std::vector<Word*>);

            Spell(){};
            Spell(std::vector<Word*>);
            Spell(std::vector<Rune*> components_);
            ~Spell(void);

            const std::vector<Rune*> components(void) const;
            bool edit(std::vector<Rune*>);



            const Word* component(natural) const;
            Word* component(natural, Word*);
            void removeComponent(int);
            void addComponent(Word*, bool = false);

            bool isValid_Deprecated(bool = false) const;
            bool edit_Deprecated(std::vector<Word*>);
            bool resolve_Deprecated(void);
            int cast_Deprecated(Mob*, BattleField*);
            const std::vector<Word*> components_Deprecated(void) const;

        private:
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
