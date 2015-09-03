#ifndef VERB_H_INCLUDED
#define VERB_H_INCLUDED

#include "../play/combatable.h"
#include "modifier.h"
#include "word.h"
#include "../globalConstants.h"

namespace Magic
{
    class Verb;
    typedef void (*VerbAction)(const Verb* context, Combatable*, Combatable*, int, int, SpellData&);
    class Verb : public Word
    {
        public:
            Verb(){};
            Verb(Rune* action);
            Verb(Rune* aux, Rune* action);
            Action action(void);
            WordType type(void) const;
            float enemyCostMultiplier(void) const;
            float allyEffectMultiplier(void) const;
            float isSameMultiplier(void) const;
            virtual int addEffect(void) const;
            virtual int addCost(void) const;
            virtual int addDuration(void) const;

            bool isBoon(void) const;
            void performAction(Combatable*, Combatable*, int, int, SpellData&) const;

        private:
            Action _action;
            Modality _modality = Modality::NA;

            VerbAction _actionWrapper;

            int _actionEffectBonus;
            int _actionCostBonus;
            int _actionDurationBonus;
            float _enemyCostMultiplier;
            float _allyEffectMultiplier;
            float _isSameMultiplier;
            bool _isBoon;

            friend void verbAct(const Verb*, Combatable*, Combatable*, int, int, SpellData&);
            friend void auxVerbAct(const Verb*, Combatable*, Combatable*, int, int, SpellData&);
    };

    void verbAct(const Verb*, Combatable*, Combatable*, int, int, SpellData&);
    void auxVerbAct(const Verb*, Combatable*, Combatable*, int, int, SpellData&);
}

#endif
