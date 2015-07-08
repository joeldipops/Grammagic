#ifndef NOUN_H_INCLUDED
#define NOUN_H_INCLUDED

#include <vector>
#include "word.h"
#include "modifier.h"
#include "../mapObject.h"
#include "battleField.h"

class Mob;

typedef std::vector<MapObject*> (*MultiTargeter) (Mob*, BattleField*);

class Noun : public Word
{
    public:
        Noun(){};
        Noun(MultiTargeter, std::string, Modifier, Modifier, Modifier);
        std::vector<MapObject*> acquireCandidates(Mob*, BattleField*);

    private:
        MultiTargeter _targeter;


};

#endif
