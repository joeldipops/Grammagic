#ifndef VERB_H_INCLUDED
#define VERB_H_INCLUDED

#include "../mapObject.h"
#include "modifier.h"
#include "word.h"
#include "../globalConstants.h"

namespace Magic
{
    typedef void (*Action) (MapObject*, MapObject*, int, int);

    class Verb : public Word
    {
        public:
            Verb(){};
            Verb(Action, std::string, Modifier, Modifier, Modifier, float = 1, float = 1, float = 1);
            Action action(void);
            WordType type(void) const;
            float enemyCostMultiplier(void) const;
            float allyEffectMultiplier(void) const;
            float isSameMultiplier(void) const;

        private:
            Action _action;
            float _enemyCostMultiplier;
            float _allyEffectMultiplier;
            float _isSameMultiplier;
    };
}

#endif
