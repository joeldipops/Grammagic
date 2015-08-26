#ifndef NOUNISH_H_INCLUDED
#define NOUNISH_H_INCLUDED

#include "word.h"
#include "../mapObject.h"
#include "../play/battleField.h"


namespace Play { class Mob; }
using namespace Play;
namespace Magic
{
    // Abstract class
    class Nounish : public Word
    {
        public:
            Nounish(){};
            Nounish(std::string name, Modifier effect, Modifier cost, Modifier duration) : Word(name, effect, cost, duration){};

            virtual const std::vector<Word*> components_Deprecated(void) const = 0;
            virtual Combatable* acquireTarget(Mob*, BattleField*) = 0;
    };
}
#endif
