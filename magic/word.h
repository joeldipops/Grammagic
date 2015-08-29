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

            // Deprecated
            Word(std::string, Modifier, Modifier, Modifier);
            const std::string& name(void) const;
            virtual bool isDummy(void) const;
            const virtual Modifier* effect(void) const;
            const virtual Modifier* cost(void) const;
            const virtual Modifier* duration(void) const;

        protected:
            // Deprecated
            Modifier effect(Modifier);
            Modifier cost(Modifier);
            Modifier duration(Modifier);

        private:
            int _addEffect = 0;
            int _addCost = 0;
            int _addDuration = 0;

            float _modEffect = 1;
            float _modCost = 1;
            float _modDuration = 1;

            // Deprecated
            std::string _name;
            Modifier _effect;
            Modifier _cost;
            Modifier _duration;
    };
}
#endif
