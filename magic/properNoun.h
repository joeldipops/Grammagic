#ifndef PROPERNOUN_H_INCLUDED
#define PROPERNOUN_H_INCLUDED

#include "nounish.h"
#include "../mapObject.h"
#include "battleField.h"
#include "modifier.h"

class Mob;
namespace Magic
{
    typedef MapObject* (*Targeter)(Mob*, BattleField*);

    class ProperNoun : public Nounish
    {
        public:
            ProperNoun(){};
            ProperNoun(Targeter, std::string, Modifier, Modifier, Modifier);

            MapObject* acquireTarget(Mob*, BattleField*);
        private:
            Targeter _target;

    };
}

#endif
