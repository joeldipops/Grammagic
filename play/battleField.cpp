#include "battleField.h"
#include "../play/mob.h"
#include "../play/gameMap.h"

using namespace Magic;

BattleField::BattleField(GameMap* map_)
{
    _pcs.push_back(map_->pc());

    for(unsigned int i = 1; i < map_->contents().size(); i++)
    {
        if (!map_->contents().at(i)->isMob())
            continue;
        Mob* m = (Mob*) map_->contents().at(i);

        if (m->isSeen(*map_->pc()))
            _hostiles.push_back(m);
    }

    std::vector<void*> _rubbishBin = std::vector<void*>(0);
}

/**
 * Garbage collects any memory that was allocated during combat.
 */
BattleField::~BattleField(void)
{
    for(Combatable* item : _rubbishBin)
    {
        delete item;
    }
    _rubbishBin = std::vector<Combatable*>(0);
}

/**
 * Return true if a victory condition (for the player) has been met.
 */
bool BattleField::isVictory(void) const
{
    for (Combatable* m : _hostiles)
    {
        if (m->stamina() > 0)
            return false;
    }

    return true;
}

/**
 * Return true if a game over condition has been met.
 */
bool BattleField::isDefeat(void) const
{
    for (Combatable* m : _pcs)
    {
        if (m->stamina() > 0)
            return false;
    }

    return true;
}

/**
 *
 */
std::vector<Mob*> BattleField::getDue(void)
{
    std::vector<Mob*> result = std::vector<Mob*>(0);
    if (!_isInCombat)
        return result;

    int time = SDL_GetTicks();
    for (Combatable* c : combatants())
    {
        Mob* item = (Mob*) c;
        // Now is the time to clean up dead mobs.
        if (item->stamina() <= 0)
        {
            unsigned int i;
            for (i = 0; i < _hostiles.size(); i++)
                if (item == _hostiles.at(i))
                    _hostiles.erase(_hostiles.begin() + i);
            for (i = 0; i < _pcs.size(); i++)
                if (item == _pcs.at(i))
                    _pcs.erase(_pcs.begin() + i);
            continue;
        }

        if (item->tryUnblock(time) || !item->isBlocked())
            result.push_back(item);
    }
    result.shrink_to_fit();
    return result;
}

bool BattleField::areAllied(const Combatable* one, const Combatable* other) const
{
    int found = 0;
    for(Combatable* mob : _hostiles)
    {
        if (mob == one)
            found++;
        if (mob == other)
            found++;
    }
    return found % 2 == 0;
}

bool BattleField::isInCombat(bool isInCombat_)
{
    _isInCombat = isInCombat_;
    return _isInCombat;
}
bool BattleField::isInCombat(void) const
{
    return _isInCombat;
}

std::vector<Mob*> BattleField::hostiles(void)
{
    return _hostiles;
}

std::vector<Mob*> BattleField::pcs(void)
{
    return _pcs;
}

std::vector<Combatable*> BattleField::combatants(void)
{
    std::vector<Combatable*> result = std::vector<Combatable*>();

    for (Mob* m : _hostiles)
    {
        result.push_back(m);
    }
    for (Mob* m : _pcs)
    {
        result.push_back(m);
    }
    return result;
}

/**
 * Keep track of an obj that was allocated during combat.
 */
void BattleField::toBin(Combatable* rubbish)
{
    _rubbishBin.push_back(rubbish);
}
