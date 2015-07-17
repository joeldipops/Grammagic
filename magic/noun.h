#ifndef NOUN_H_INCLUDED
#define NOUN_H_INCLUDED

#include <vector>
#include "word.h"
#include "modifier.h"
#include "../mapObject.h"
#include "../play/battleField.h"

class Mob;
namespace Magic
{
    typedef std::vector<MapObject*> (*MultiTargeter) (Mob*, BattleField*);

    class Noun : public Word
    {
        public:
            Noun(){};
            Noun(MultiTargeter, std::string, Modifier, Modifier, Modifier);
            std::vector<MapObject*> acquireCandidates(Mob*, BattleField*);
            WordType type(void) const;

        private:
            MultiTargeter _targeter;
    };
}

#endif
