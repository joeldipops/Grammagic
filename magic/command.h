#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

#include "verb.h"
#include "../menuItem.h"


namespace Play { class BattleField; class Mob; }

namespace Magic
{
    class Command : public MenuItem
    {
        public:
            Command(void){};
            Command(const std::string&, Effect);
            ~Command(void);

            virtual bool isValid(void) const { return true; };
            virtual const std::vector<Rune*> components(void) const;
            virtual int execute(Mob*, SpellContext&);

        private:
            Effect _effect;
            bool _isSpell = false;
    };
}

#endif
