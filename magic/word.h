#ifndef WORD_H_INCLUDED
#define WORD_H_INCLUDED

#include <string>
#include "../globalConstants.h"
#include "modifier.h"

namespace Magic
{
    class Word
    {
        public:
            Word(){};
            Word(std::string, Modifier, Modifier, Modifier);
            virtual ~Word();
            const std::string name(void) const;
            const virtual Modifier* effect(void) const;
            const virtual Modifier* cost(void) const;
            const virtual Modifier* duration(void) const;
            virtual WordType type(void) const = 0;

        protected:
            Modifier effect(Modifier);
            Modifier cost(Modifier);
            Modifier duration(Modifier);

        private:
            std::string _name;
            Modifier _effect;
            Modifier _cost;
            Modifier _duration;
    };
}
#endif
