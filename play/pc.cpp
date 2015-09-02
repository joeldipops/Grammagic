#include "pc.h"

using namespace Play;


//{ Lifecycle
PC::PC(const Templates::PCTemplate& tmpl)
: Mob(tmpl, MobType::PlayerCharacter)
{
    _name = tmpl.Name;
    _memberCode = tmpl.MemberCode;
    otherCommands().push_back(new Command("Flee", Templates::Commands::FLEE));
    _jobClass = JobClass();
}

PC::~PC(void)
{
}
//}

//{ Properties
/**
 * Returns the maximum number of runes per spell a pc can have.
 */
natural PC::runeSlots(void) const { return _runeSlots; }

/**
 * The character's name.
 */
std::string PC::name(void) const { return _name; }

/**
 * Identifies which character this mob represents.
 */
Templates::PartyMemberCode PC::memberCode(void) const { return _memberCode; }

//}

//{ Methods
/**
 * Returns the maximum number of spells a pc can keep at one time.
 */
natural PC::spellSlots(void) const
{
    return _spellSlots > spells().size()
    ? spells().size() + 1
    : _spellSlots;
}

std::string PC::className(void) const
{
    return _jobClass.name();
}

bool PC::isPlayerControlled(void) const
{
    return true;
}

//}



