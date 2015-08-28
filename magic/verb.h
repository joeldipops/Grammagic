#ifndef VERB_H_INCLUDED
#define VERB_H_INCLUDED

#include "../play/combatable.h"
#include "modifier.h"
#include "word.h"
#include "../globalConstants.h"

namespace Magic
{
    class Verb;
    typedef void (*VerbAction)(Verb* context, Combatable*, Combatable*, int, int, SpellData&);
    class Verb : public Word
    {
        public:
            Verb(){};
            Verb(Rune* action);
            Verb(Rune* aux, Rune* action);
            Verb(Action, std::string, Modifier, Modifier, Modifier, float = 1, float = 1, float = 1, bool = false);
            Action action(void);
            WordType type(void) const;
            float enemyCostMultiplier(void) const;
            float allyEffectMultiplier(void) const;
            float isSameMultiplier(void) const;
            bool isBoon(void) const;
            void performAction(Combatable*, Combatable*, int, int);

        private:
            Action _action;
            Modality _modality = Modality::NA;

            VerbAction _actionWrapper;

            friend void verbAct(Verb*, Combatable*, Combatable*, int, int, SpellData&);
            friend void auxVerbAct(Verb*, Combatable*, Combatable*, int, int, SpellData&);

            // deprecated
            float _enemyCostMultiplier;
            float _allyEffectMultiplier;
            float _isSameMultiplier;
            bool _isBoon;
    };

    void verbAct(Verb*, Combatable*, Combatable*, int, int, SpellData&);
    void auxVerbAct(Verb*, Combatable*, Combatable*, int, int, SpellData&);
}

#endif
