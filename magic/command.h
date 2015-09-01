#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

#include "spell.h"
#include "verb.h"
#include "../menuItem.h"


namespace Play { class BattleField; class Mob; }

namespace Magic
{

    class Command;

    class Command : public MenuItem
    {
        public:
            Command(void){};
            Command(std::string, Effect);
            Command(std::string, Spell);
            ~Command(void);

            const std::string& name(void) const;
            const std::vector<Rune*> components(void) const;
            int execute(Mob*, BattleField*);
            Spell* spell(void);
            const Spell* spell(void) const;

        private:
            Effect _effect;
            Spell _spell;
            bool _hasSpell = false;
            static int spellCaster(Command* _this, Mob* caster, BattleField*);
    };
}

#endif
