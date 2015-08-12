#include "mob.h"

using namespace Magic;

Mob::Mob() {}

/**
 * Constructs a mob of the given type.
 * @param type A mob type, such as PC or Demon Monster Firefighter.
 */
Mob::Mob(MobType type)
{
    if (type != MobType::None)
        isDense(true);
    else
        isDense(false);

    _spellCommands = std::vector<Command>();
    _otherCommands = std::vector<Command>();

    _maxStamina = 100;
    _stamina = 100;
    _rangeOfSight = 1;
    _rangeOfSense = 3;
    _type = type;
    switch(type)
    {
        case MobType::PlayerCharacter:
        case MobType::Hostile:
            break;
        default:
            imageFileName(RESOURCE_LOCATION + "hidden.png");
            break;
    }
}

/**
 * Destructor
 */
Mob::~Mob()
{
}

/**
 * Gets the type mob this is.
 */
MobType Mob::type(void) const
{
    return _type;
}

/**
 * Gets the time, in milliseconds, at which a mob can perform commands again.
 */
int Mob::unblockTime(void) const
{
    return _unblockTime;
}

/**
 * returns true if the mob is blocked from performing commands, otherwise false.
 */
bool Mob::isBlocked(void) const
{
    return _isBlocked;
}

/**
 * Blocks the mob from performing commands until a given amount of time has passed.
 * @param the time in milliseconds since program-start that the mob can perform again.
 */
void Mob::block(int unblockTime_)
{
    _blockedTime = SDL_GetTicks();
    _isBlocked = true;
    _unblockTime = unblockTime_;
}

/**
 * @return The percentage of time elapsed until can be unblocked.
 */
int Mob::elapsedWait(void) const
{
    int ticks = SDL_GetTicks();
    if (_unblockTime > ticks)
    {
        int elapsed = ticks - _blockedTime;
        int total = _unblockTime - _blockedTime;

        return elapsed / (double)total * 100;
    }

    return -1;
}

/**
 * Checks if can be unblocked and unblocks if appropriate.
 * @param now The time to check against.
 * @return true if state was changed from blocked to unblocked, false otherwise.
 * (false if mob had already been unblocked)
 */
bool Mob::tryUnblock(int now)
{
    if (!_isBlocked)
        return false;

    if (_unblockTime <= now)
    {
        _isBlocked = false;
        return true;
    }

    return false;
}

/**
 * Allows the mob to act immediately
 */
void Mob::unblock(void)
{
    _isBlocked = false;
    _unblockTime = 0;
}

int Mob::stamina(int stamina_)
{
    _stamina = stamina_;
    return stamina();
}
int Mob::stamina(void) const
{
    return _stamina;
}

int Mob::maxStamina(void) const
{
    return _maxStamina;
}

/**
 * Adds the passed in value to the mob's current stamina (pass in a negative to reduce it)
 * @param delta The change in stamina.
 * @return The new stamina.
 */
int Mob::changeStamina(int delta)
{
    int newStamina = _stamina + delta;

    if (newStamina < 0)
        _stamina = 0;
    else if (newStamina > _maxStamina)
        _stamina = _maxStamina;
    else
        _stamina = newStamina;

    return _stamina;
}

/**
 * Gets or sets the path to the image that represents this mob.
 * @param portraitFileName The file name.
 * @return The file name.
 */
const std::string Mob::portraitFileName(void) const
{
    return _portraitFileName;
}
std::string Mob::portraitFileName(std::string portraitFileName_)
{
    _portraitFileName = portraitFileName_;
    return _portraitFileName;
}

/**
 * Gets and sets the distance at which the enemy will start attacking you.
 */
int Mob::rangeOfSight(int range)
{
    _rangeOfSight = range;
    return _rangeOfSight;
}
int Mob::rangeOfSight(void) const
{
    return _rangeOfSight;
}

/**
 * Gets and sets the distance at which the enemy will start chasing you.
 */
int Mob::rangeOfSense(int range)
{
    _rangeOfSense = range;
    return _rangeOfSense;
}
int Mob::rangeOfSense(void) const
{
    return _rangeOfSense;
}

std::vector<Command*> Mob::commands(void) const
{
    std::vector<Command*> result = std::vector<Command*>();

    for (unsigned int i = 0; i < _otherCommands.size(); i++)
    {
        result.push_back(const_cast<Command*>(&_otherCommands.at(i)));
    }

    for (unsigned int i = 0; i < _spellCommands.size(); i++)
    {
        result.push_back(const_cast<Command*>(&_spellCommands.at(i)));
    }
    return result;
}

std::vector<Command>* Mob::spells(void)
{
    return &_spellCommands;
}

const std::vector<Command>* Mob::spells(void) const
{
    return &_spellCommands;
}


/**
 * Gets the last selected command.
 * @return A command.
 */
Command* Mob::selectedCommand(void) const
{
    return commands()[_selectedCommandIndex];
}

/**
 * @return get ordered index of the selected command.
 */
int Mob::selectedCommandIndex(void) const
{
    return _selectedCommandIndex;
}

/**
 * Sets the selected command.
 * @param index The index of the command.
 * @return The newly set commandIndex, or -1 if operation failed.
 */
int Mob::selectedCommandIndex(unsigned int index)
{
    if (index < 0 || index >= commands().size())
        return -1;

    _selectedCommandIndex = index;
    return _selectedCommandIndex;
}



bool Mob::isSensed(const MapObject& target) const
{
    return isInRange(target, _rangeOfSense);
}

/**
 * Checks whether a mob can be seen.
 * @param target The mob that may or may not be visible.
 * @return True if the given mob can be seen by this Mob, false otherwise.
 */
bool Mob::isSeen(const MapObject& target) const
{
    return isInRange(target, _rangeOfSight);
}

bool Mob::isInRange(const MapObject& target, int value) const
{
    if (&target == this)
        return false;
    return (abs(target.x() - x()) <= value)
        && (abs(target.y() - y()) <= value);
}

/**
 * @return The list of commands that can be cast as spells.
 */
std::vector<Command>* Mob::spellCommands()
{
    return &_spellCommands;
}

/**
 * @return The list of commands that are not spells.
 */
std::vector<Command>* Mob::otherCommands()
{
    return &_otherCommands;
}

/**
 * The modified defence stat.
 */
float Mob::defence(void) const
{
    return _defaultDefence * _defenceMultiplier;
}

/**
 * Gets or sets the mob's default resistance stat, which is a multiplier.
 */
float Mob::defaultDefence(void) const
{
    return _defaultDefence;
}
float Mob::defaultDefence(float defaultDefence_)
{
    _defaultDefence = defaultDefence_;
    return _defaultDefence;
}

/**
 * Multiplier that increases or reduces physical damage.
 */
float Mob::changeDefence(float multiplier)
{
    if (multiplier >= 0)
        _defenceMultiplier = multiplier;
    return defence();
}

/**
 * The modified resistance stat.
 */
float Mob::resistance(void) const
{
    return _defaultResistance * _resistanceMultiplier;
}

/**
 * Gets or sets the mob's default resistance stat, which is a multiplier.
 */
float Mob::defaultResistance(void) const
{
    return _defaultResistance;
}
float Mob::defaultResistance(float defaultResistance_)
{
    _defaultResistance = defaultResistance_;
    return _defaultResistance;
}

float Mob::changeResistance(float multiplier)
{
    if (multiplier >= 0)
        _resistanceMultiplier = multiplier;
    return resistance();
}


/**
 * The modified skill stat.
 */
float Mob::skill(void) const
{
    return _defaultSkill * _skillMultiplier;
}

/**
 * Gets or sets the mob's default resistance stat, which is a multiplier.
 */
float Mob::defaultSkill(void) const
{
    return _defaultSkill;
}
float Mob::defaultSkill(float defaultSkill_)
{
    _defaultSkill = defaultSkill_;
    return _defaultSkill;
}

float Mob::changeSkill(float multiplier)
{
    if (multiplier >= 0)
        _skillMultiplier = multiplier;
    return skill();
}

/**
 * @return The possibly modified speed stat.
 */
float Mob::speed(void) const
{
    return _defaultSpeed * _speedMultiplier;
}

/**
 * Gets or sets the mob's default speed stat, which is a multiplier.
 * @param defaultSpeed the new speed stat.
 * @return the current speed stat.
 */
float Mob::defaultSpeed(float defaultSpeed_)
{
    _defaultSpeed = defaultSpeed_;
    return _defaultSpeed;
}
float Mob::defaultSpeed(void) const
{
    return _defaultSpeed;
}

/**
 * Temporary modifies the speed stat by a multiplier.
 * @param multiplier A non-negative double.
 * @return the modified speed stat.
 */
float Mob::changeSpeed(float multiplier)
{
    if (multiplier >= 0)
        _speedMultiplier = multiplier;
    return speed();
}

/**
 * After combat ends, reset any modified stats etc.
 */
void Mob::endCombat(void)
{
    unblock();
    _speedMultiplier = 1.0;
    _resistanceMultiplier = 1.0;
    _defenceMultiplier = 1.0;
    _skillMultiplier = 1.0;
}


