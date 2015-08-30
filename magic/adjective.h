#ifndef ADJECTIVE_H_INCLUDED
#define ADJECTIVE_H_INCLUDED

#include <vector>

#include "word.h"
#include "../play/mapObject.h"
#include "../play/battleField.h"

namespace Play { class Mob; }

using namespace Play;
namespace Magic
{
    class Adjective : public Word {
        public:
            Adjective(){};
            Adjective(Selecter, std::string, Modifier, Modifier, Modifier);
            Combatable* SelectTargetFromCandidates(Mob*, BattleField*, std::vector<Combatable*>);
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

