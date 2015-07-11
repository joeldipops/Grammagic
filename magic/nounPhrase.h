#ifndef NOUNPHRASE_H_INCLUDED
#define NOUNPHRASE_H_INCLUDED

#include "nounish.h"
#include "adjective.h"
#include "noun.h"
namespace Magic
{
    class NounPhrase : public Nounish
    {
        public:
            NounPhrase(){};
            NounPhrase(Noun, Adjective);

            // Override base
            MapObject* acquireTarget(Mob*, BattleField*);
            //const virtual Modifier* effect(void) const;
            //const virtual Modifier* cost(void) const;
            //const virtual Modifier* duration(void) const;

        private:
            Noun _targeter;
            Adjective _specifier;
    };
}
#endif
