#ifndef ADVERB_H_INCLUDED
#define ADVERB_H_INCLUDED

#include "../globalConstants.h"
#include "word.h"

namespace Magic
{
    class Adverb : public Word
    {
        public:
            Adverb() {};
            Adverb(Rune* adv);
            WordType type(void) const;

            SpellData& modifySpell(SpellData&);

        private:
            MetaAction _modify;
    };
}

#endif
