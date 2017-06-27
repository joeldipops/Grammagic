#include "mob.h"
#include "playStateManager.h"

using namespace Magic;
using namespace Play;

//{ Lifecycle
/**
 * Constructor
 */
Mob::Mob(const Resources::MobTemplate& tmpl, MobType type_)
    :MapObject(tmpl)
{
    _type = type_;
    _maxStamina = tmpl.Stamina;
    _stamina = _maxStamina;
    _rangeOfSight = tmpl.RangeOfSight;
    _rangeOfSense = tmpl.RangeOfSense;
    defaultSpeed(tmpl.Speed);
    defaultSkill(tmpl.Skill);
    defaultResistance(tmpl.Resistance);
    defaultDefence(tmpl.Defence);
    portraitFileName(tmpl.PortraitPath);
    imageFileName(tmpl.ImagePath);

    if (type_ != MobType::None)
        isDense(true);
    else
        isDense(false);

    _spellCommands = std::vector<Command*>();
    _otherCommands = std::vector<Command*>();
    facing(Direction::SOUTH);
}

/**
 * Destructor
 */
Mob::~Mob()
{
    for (Command* c : _spellCommands)
        deletePtr(c);

    _spellCommands = std::vector<Command*>();

    for (Command* c : _otherCommands)
        deletePtr(c);

    _otherCommands = std::vector<Command*>();
}
//}

//{ Properties
/**
 * Gets the type mob this is.
 */
MobType Mob::type(void) const { return _type; }

/**
 * Gets the time, in milliseconds, at which a mob can perform commands again.
 */
int Mob::unblockTime(void) const { return _unblockTime; }

/**
 * returns true if the mob is blocked from performing commands, otherwise false.
 */
bool Mob::isBlocked(void) const { return _isBlocked; }

/**
 * When stamina drops to 0 in combat, mob can't perform any more actions.
 */
int Mob::stamina(void) const { return _stamina; }
int Mob::stamina(int stamina_)
{
    _stamina = stamina_;
    return stamina();
}

unsigned short Mob::maxStamina(void) const { return _maxStamina; }
unsigned short Mob::maxStamina(unsigned short maxStamina_)
{
    return _maxStamina = maxStamina_;
}

/**
 * Gets or sets the mob's default speed stat, which is a multiplier.
 * @param defaultSpeed the new speed stat.
 * @return the current speed stat.
 */
float Mob::defaultSpeed(void) const { return _defaultSpeed; }
float Mob::defaultSpeed(float defaultSpeed_)
{
    _defaultSpeed = defaultSpeed_;
    return _defaultSpeed;
}

/**
 * Gets or sets the mob's default resistance stat, which is a multiplier.
 */
float Mob::defaultDefence(void) const { return _defaultDefence; }
float Mob::defaultDefence(float defaultDefence_)
{
    _defaultDefence = defaultDefence_;
    return _defaultDefence;
}

/**
 * Gets or sets the mob's default resistance stat, which is a multiplier.
 */
float Mob::defaultSkill(void) const { return _defaultSkill; }
float Mob::defaultSkill(float defaultSkill_)
{
    _defaultSkill = defaultSkill_;
    return _defaultSkill;
}

/**
 * Gets or sets the mob's default resistance stat, which is a multiplier.
 */
float Mob::defaultResistance(void) const { return _defaultResistance; }
float Mob::defaultResistance(float defaultResistance_)
{
    _defaultResistance = defaultResistance_;
    return _defaultResistance;
}

const std::vector<Command*>& Mob::spells(void) const { return _spellCommands; }

/**
 * Gets and sets the distance at which the enemy will start attacking you.
 */
int Mob::rangeOfSight(void) const { return _rangeOfSight; }
int Mob::rangeOfSight(int range)
{
    _rangeOfSight = range;
    return _rangeOfSight;
}

/**
 * Gets and sets the distance at which the enemy will start chasing you.
 */
int Mob::rangeOfSense(void) const { return _rangeOfSense; }
int Mob::rangeOfSense(int range)
{
    _rangeOfSense = range;
    return _rangeOfSense;
}

/**
 * @return The list of commands that are not spells.
 */
std::vector<Command*>& Mob::otherCommands() { return _otherCommands; }

//}

//{ Methods
void Mob::addSpell(Spell* spell)
{
    _spellCommands.push_back(spell);
}

PlayStateContainer& Mob::onInspect(PlayStateContainer& data)
{
    if (onInspectFn())
    {
        const auto fn =  onInspectFn();
        return fn(this, data);
    }
    return data;
}

/**
 * Removes any spells with no components that may have been added in error.
 */
void Mob::cleanUpSpellList(void)
{
    std::vector<Command*> temp = std::vector<Command*>();
    for (Command* s : _spellCommands)
    {
        if (s->components().size() <= 0)
            delete s;
        else
            temp.push_back(s);
    }
    _spellCommands = temp;
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
const std::string& Mob::portraitFileName(void) const
{
    return _portraitFileName;
}
const std::string& Mob::portraitFileName(const std::string& portraitFileName_)
{
    _portraitFileName = portraitFileName_;
    return _portraitFileName;
}

std::vector<Command*> Mob::commands(void) const
{
    std::vector<Command*> result = std::vector<Command*>();
    result.insert(result.end(), _otherCommands.begin(), _otherCommands.end());
    result.insert(result.end(), _spellCommands.begin(), _spellCommands.end());

    return result;
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
int Mob::selectedCommandIndex(natural index)
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
 * The modified defence stat.
 */
float Mob::defence(void) const
{
    return _defaultDefence * _defenceMultiplier;
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
    isInCombat(false);
    _speedMultiplier = 1.0;
    _resistanceMultiplier = 1.0;
    _defenceMultiplier = 1.0;
    _skillMultiplier = 1.0;
}

//}


