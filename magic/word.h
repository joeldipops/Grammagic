#ifndef WORD_H_INCLUDED
#define WORD_H_INCLUDED

#include <string>

#include "../globalConstants.h"
#include "rune.h"
#include "modifier.h"
#include "magicTypedefs.h"

namespace Magic
{
    class Word
    {
        public:
            Word(void){};
            Word(const std::vector<Rune*>);

            virtual ~Word();

            virtual WordType type(void) const = 0;

            virtual int addEffect(void) const;
            virtual int addCost(void) const;
            virtual int addDuration(void) const;

            float modEffect(void) const;
            float modCost(void) const;
            float modDuration(void) const;

        private:
            int _addEffect = 0;
            int _addCost = 0;
            int _addDuration = 0;

            float _modEffect = 1;
            float _modCost = 1;
            float _modDuration = 1;
    };
}
#endif
