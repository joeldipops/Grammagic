#ifndef PROPERNOUN_H_INCLUDED
#define PROPERNOUN_H_INCLUDED

#include "nounish.h"
#include "../mapObject.h"
#include "../play/battleField.h"
#include "modifier.h"

class Mob;
namespace Magic
{
    typedef Combatable* (*Targeter)(Mob*, BattleField*);

    class ProperNoun : public Nounish
    {
        public:
            ProperNoun(){};
            ProperNoun(Targeter, std::string, Modifier, Modifier, Modifier);

            WordType type(void) const;
            const std::vector<Word*> components(void) const;

            Combatable* acquireTarget(Mob*, BattleField*);
        private:
            Targeter _target;

    };
}

#endif
