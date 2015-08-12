#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

#include "spell.h"
#include "properNoun.h"
#include "verb.h"
#include "../menuItem.h"


namespace Play { class BattleField; class Mob; }

namespace Magic
{

    class Command;

    /**
    * Casts a spell or carries out some other in-combat command.
    * @param _this The command being carried out.
    * @param battlefield holds various data on combat currently in progress.
    * @return The time before the mob that made the command can act again.
    */

    typedef int (*Effect)(Command* _this, Mob*, BattleField*);

    class Command : public MenuItem
    {
        public:
            Command(void){};
            Command(std::string, Effect);
            Command(std::string, Spell);
            ~Command(void);

            std::string name(void) const;
            const std::vector<Word*> components(void) const;
            int execute(Mob*, BattleField*);
            bool edit(std::vector<Word*> components);
            bool isValid(void) const;
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
