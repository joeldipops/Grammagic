#include "pc.h"

using namespace Play;

PC::PC(const Templates::PCTemplate& tmpl)
: Mob(tmpl, MobType::PlayerCharacter)
{
    _name = tmpl.Name;
    _memberCode = tmpl.MemberCode;
    otherCommands()->push_back(Command("Flee", Commands::FLEE));
    _runeSlots = 5;
    _spellSlots = 7;
    _jobClass = JobClass();
}

/**
 * Returns the maximum number of spells a pc can keep at one time.
 */
int PC::spellSlots(void) const
{
    return _spellSlots > int(spells()->size())
    ? spells()->size() + 1
    : _spellSlots;
}

/**
 * Returns the maximum number of runes per spell a pc can have.
 */
int PC::runeSlots(void) const { return _runeSlots; }

/**
 * The character's name.
 */
std::string PC::name(void) const { return _name; }

/**
 * Identifies which character this mob represents.
 */
Templates::PartyMemberCode PC::memberCode(void) const { return _memberCode; }

std::string PC::className(void) const
{
    return _jobClass.name();
}

bool PC::isPlayerControlled(void) const
{
    return true;
}
