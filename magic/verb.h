#ifndef VERB_H_INCLUDED
#define VERB_H_INCLUDED

#include "../mapObject.h"
#include "modifier.h"
#include "word.h"
#include "../globalConstants.h"

namespace Magic
{
    typedef void (*Action) (MapObject*, MapObject*, double, double);

    class Verb : public Word
    {
        public:
            Verb(){};
            Verb(Action, std::string, Modifier, Modifier, Modifier, double = 1, double = 1, double = 1);
            Action action(void);
            WordType type(void) const;
            double enemyCostMultiplier(void) const;
            double allyEffectMultiplier(void) const;
            double isSameMultiplier(void) const;

        private:
            Action _action;
            double _enemyCostMultiplier;
            double _allyEffectMultiplier;
            double _isSameMultiplier;
    };
}

#endif
