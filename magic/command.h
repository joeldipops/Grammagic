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

            const std::string& name(void) const;
            virtual const std::vector<Rune*> components(void) const;
            virtual int execute(Mob*, BattleField&);

        private:
            Effect _effect;
            bool _isSpell = false;
    };
}

#endif
