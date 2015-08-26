#ifndef PROPERNOUN_H_INCLUDED
#define PROPERNOUN_H_INCLUDED

#include "nounish.h"
#include "../mapObject.h"
#include "../play/battleField.h"
#include "modifier.h"

namespace Play { class Mob; }

namespace Magic
{
    class ProperNoun : public Nounish
    {
        public:
            ProperNoun(){};
            ProperNoun(Rune* noun);
            ProperNoun(Targeter, std::string, Modifier, Modifier, Modifier);

            WordType type(void) const;
            const std::vector<Word*> components_Deprecated(void) const;

            Combatable* acquireTarget(Mob*, BattleField*);
        private:
            Targeter _target;

    };
}

#endif
