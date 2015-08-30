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
    return _this->_spell.cast_Deprecated(caster, field);
}

/**
 * Constructor
 */
Command::Command(std::string name, Spell spell) : MenuItem(name)
{
    _hasSpell = true;
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
 * Destructor
 */
Command::~Command()
{

}

/**
 * @return the wrapped spell.
 */
Spell* Command::spell(void)
{
    return &_spell;
}
const Spell* Command::spell(void) const
{
    return &_spell;
}

/**
 * Overrides base to return the name of the wrapped spell.
 */
const std::string& Command::name(void) const
{
    for(Word* w : components_Deprecated())
    {
        if (w->type() == WordType::VERB)
            return w->name();
    }

    return MenuItem::name();
}

/**
 * Returns the rune components of an attached spell, if any.
 */
const std::vector<Word*> Command::components_Deprecated(void) const
{
    if (_hasSpell)
        return _spell.components_Deprecated();
    return std::vector<Word*>(0);
}


bool Command::edit_Deprecated(std::vector<Word*> components_Deprecated)
{
    return _spell.edit_Deprecated(components_Deprecated);
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
