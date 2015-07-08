#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

#include "spell.h"
#include "properNoun.h"
#include "verb.h"
#include "../menuItem.h"
class Mob;
class BattleField;
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
        int execute(Mob*, BattleField*);

    private:
        Effect _effect;
        Spell _spell;
        static int spellCaster(Command* _this, Mob* caster, BattleField*);
};

#endif
