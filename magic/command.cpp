#include "command.h"
using namespace Magic;
/**
 * Static function that executes spell type commands with the appropriate context.
 * @param _this the command used to cast the spell.
 * @param caster The mob that casted the spell.
 *
 * @return milliseconds to wait before the caster can act again.
 */
int Command::spellCaster(Command* _this, Mob* caster, BattleField* field)
{
    return _this->_spell.cast(caster, field);                         sdfsfsfdsf
}


/**
 * Constructor
 */
Command::Command(std::string name, Spell spell) : MenuItem(name)
{
    _spell = spell;
    _effect = spellCaster;
}

/**
 * Constructor
 * @param name The displayed name of the command.
 * @param effect The action carried out by the command.
 */
Command::Command(std::string name, Effect effect) : MenuItem(name)
{
    _effect = effect;
}

/**
 * Executes the action of this command on behalf of a particular mob.
 * @param caster The mob who issued the command.
 * @param combatants All mobs involved in the current battle.
 * @return Number of milliseconds this command should wait after executing.
 */
int Command::execute(Mob* caster, BattleField* field)
{
    return _effect(this, caster, field);
}
