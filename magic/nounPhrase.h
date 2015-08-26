#ifndef NOUN_PHRASE_H_INCLUDED
#define NOUN_PHRASE_H_INCLUDED

#include "nounish.h"
#include "adjective.h"
#include "noun.h"

namespace Magic
{
    class NounPhrase : public Nounish
    {
        public:
            NounPhrase(){};
            NounPhrase(Noun*, Adjective*);

            // Override base
            Combatable* acquireTarget(Mob*, BattleField*);
            WordType type(void) const;
            const std::vector<Word*> components_Deprecated(void) const;

        private:
            Noun* _targeter;
            Adjective* _specifier;
    };
}
#endif
