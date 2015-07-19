#include "mob.h"
#include "../magic/nounish.h"
#include "../magic/nounPhrase.h"
#include "../util/utils.h"

#include <iostream>

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
    _type = type;
    switch(type)
    {
        case MobType::PlayerCharacter:
        case MobType::Hostile:
            break;
        default:
            imageFileName(RESOURCE_LOCATION + "blank.png");
            break;
    }
}

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
 * Gets or sets the X/Y coordinates of the mob.
 * @param x
 * @param y
 * @return
 */
Location Mob::location(int x, int y)
{
    _x = x;
    _y = y;
    return Location(&x, &y);
}

/**
 *
 * @param loc
 * @return
 */
Location Mob::location(const Location* loc)
{
    if (loc != nullptr)
    {
        _x = loc->X;
        _y = loc->Y;
    }
    return Location(&_x, &_y);
}


/**
 * Gets or sets the Mob's X position.
 * @param x The horizontal co-ordinate of the map
 * @return
 */
int Mob::x(void) const
{
    return _x;
}
int Mob::x(int x_)
{
    _x = x_;
    return x();
}

/**
 * Gets or sets the Mob's Y position.
 * @param y The vertical co-ordinate on the map
 * @return
 */
int Mob::y(void) const
{
    return _y;
}
int Mob::y(int y_)
{
    _y = y_;
    return y();
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
const int Mob::stamina(void) const
{
    return _stamina;
}

const int Mob::maxStamina(void) const
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
    if (_stamina + delta <= _maxStamina)
        _stamina += delta;
    else
        _stamina = _maxStamina;
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


int Mob::rangeOfSight(int range)
{
    _rangeOfSight = range;
    return _rangeOfSight;
}
int Mob::rangeOfSight(void) const
{
    return _rangeOfSight;
}

std::vector<Command*> Mob::commands(void) const
{
    std::vector<Command*> result = std::vector<Command*>();

    for (int i = 0; i < int(_otherCommands.size()); i++)
    {
        result.push_back(const_cast<Command*>(&_otherCommands.at(i)));
    }

    for (int i = 0; i < int(_spellCommands.size()); i++)
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
int Mob::selectedCommandIndex(int index)
{
    if (index < 0 || index >= int(commands().size()))
        return -1;

    _selectedCommandIndex = index;
    return _selectedCommandIndex;
}


/**
 * Checks whether a mob can be seen.
 * @param target The mob that may or may not be visible.
 * @return True if the given mob can be seen by this Mob, false otherwise.
 */
bool Mob::isSeen(const Mob* target)
{
    return (abs(target->x() - _x) <= _rangeOfSight)
        && (abs(target->y() - _y) <= _rangeOfSight);
}

std::vector<Command>* Mob::spellCommands()
{
    return &_spellCommands;
}

std::vector<Command>* Mob::otherCommands()
{
    return &_otherCommands;
}

/**
 * @return The possibly modified speed stat.
 */
double Mob::speed(void) const
{
    return _defaultSpeed * _speedMultiplier;
}

/**
 * Gets or sets the mob's default speed stat, which is a multiplier.
 * @param defaultSpeed the new speed stat.
 * @return the current speed stat.
 */
double Mob::defaultSpeed(double defaultSpeed_)
{
    _defaultSpeed = defaultSpeed_;
    return _defaultSpeed;
}
double Mob::defaultSpeed(void) const
{
    return _defaultSpeed;
}

/**
 * Temporary modifies the speed stat by a multiplier.
 * @param multiplier A non-negative double.
 * @return the modified speed stat.
 */
double Mob::changeSpeed(double multiplier)
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
}


