#ifndef ADJECTIVE_H_INCLUDED
#define ADJECTIVE_H_INCLUDED

#include "word.h"
#include "../mapObject.h"
#include <vector>
#include "battleField.h"

class Mob;

namespace Magic {
typedef MapObject* (*Selecter) (Mob*, BattleField*, std::vector<MapObject*>);


class Adjective : public Word {
    public:
        Adjective(){};
        Adjective(Selecter, std::string, Modifier, Modifier, Modifier);
        MapObject* selectTarget(Mob*, BattleField*, std::vector<MapObject*>);
        //WordType type(void) const;

    private:
        Selecter _selecter;
};
}

#endif

