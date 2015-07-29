#include "pc.h"

PC::PC(void) : Mob(MobType::PlayerCharacter)
{
    otherCommands()->push_back(Command("Flee", Commands::FLEE));
    imageFileName(RESOURCE_LOCATION + "pc.png");
    portraitFileName(RESOURCE_LOCATION + "magician.png");
    _runeSlots = 5;
    _spellSlots = 7;
    _name = "Name";
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
int PC::runeSlots(void) const
{
    return _runeSlots;
}

/**
 * The character's name.
 */
std::string PC::name(void) const
{
    return _name;
}

std::string PC::className(void) const
{
    return _jobClass.name();
}
