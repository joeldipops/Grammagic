#ifndef NOUN_H_INCLUDED
#define NOUN_H_INCLUDED

#include <vector>

#include "word.h"
#include "modifier.h"
#include "../mapObject.h"
#include "../play/battleField.h"

namespace Play { class Mob; }
using namespace Play;
namespace Magic
{
    class Noun : public Word
    {
        public:
            Noun(){};
            Noun(Rune* properNoun);
            Noun(Rune* aux, Rune* adj, Rune* noun);
            Noun(Rune* adj, Rune* noun);
            Noun(MultiTargeter, std::string, Modifier, Modifier, Modifier);
            std::vector<Combatable*> acquireCandidates(Mob*, BattleField*);
            WordType type(void) const;

        private:
            MultiTargeter _targeter;
    };
}

#endif
