#ifndef NOUN_H_INCLUDED
#define NOUN_H_INCLUDED

#include "word.h"
#include "../mapObject.h"
#include "battleField.h"
#include "modifier.h"

class Mob;

class Noun : public Word
{
    public:
        Noun(){};
        Noun(MapObject* (*)(Mob*, BattleField*), std::string, Modifier, Modifier, Modifier);

        MapObject* aquireTarget(Mob*, BattleField*);
    private:
        MapObject* (*_target)(Mob*, BattleField*);

};

#endif
