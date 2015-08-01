#ifndef ADJECTIVE_H_INCLUDED
#define ADJECTIVE_H_INCLUDED

#include <vector>

#include "word.h"
#include "../mapObject.h"
#include "../play/battleField.h"

class Mob;
using namespace Play;
namespace Magic
{
    typedef Combatable* (*Selecter) (Mob*, BattleField*, std::vector<Combatable*>);

    class Adjective : public Word {
        public:
            Adjective(){};
            Adjective(Selecter, std::string, Modifier, Modifier, Modifier);
            Combatable* selectTarget(Mob*, BattleField*, std::vector<Combatable*>);
            WordType type(void) const;

        private:
            Selecter _selecter;
    };

    class DummyAdjective : public Adjective {
        public:
            DummyAdjective(Selecter, std::string, Modifier, Modifier, Modifier);
            bool isDummy(void) const;
    };
}

#endif

