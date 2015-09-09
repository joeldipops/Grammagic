#include "command.h"

using namespace Magic;

//{ Lifecycle
/**
 * Constructor
 * @param name The displayed name of the command.
 * @param effect The action carried out by the command.
 */
Command::Command(const std::string& name, Effect effect) : MenuItem(name)
{
    _effect = effect;
}

/**
 * Destructor
 */
Command::~Command()
{
}
//}

//{ Methods
const std::vector<Rune*> Command::components(void) const
{
    return std::vector<Rune*>(0);
}

/**
 * Overrides base to return the name of the wrapped spell.
 */
const std::string& Command::name(void) const
{
    /*
    for(Word* w : components_Deprecated())
    {
        if (w->type() == WordType::VERB)
            return w->name();
    }
    */
    return MenuItem::name();
}

/**
 * Executes the action of this command on behalf of a particular mob.
 * @param caster The mob who issued the command.
 * @param combatants All mobs involved in the current battle.
 * @return Number of milliseconds this command should wait after executing.
 */
int Command::execute(Mob* caster, BattleField& field)
{
    return _effect(this, caster, field);
}

//}
