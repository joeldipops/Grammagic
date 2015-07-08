#include "battleField.h"
#include "../play/mob.h"
#include "../play/gameMap.h"


BattleField::BattleField(GameMap* map_)
{
    _pcs.push_back(map_->pc());

    for(int i = 1; i < int(map_->mobs().size()); i++)
    {
        if (map_->mobs().at(i)->isSeen(map_->pc()))
            _hostiles.push_back(map_->mobs().at(i));
    }
}

/**
 * Return true if a victory condition (for the player) has been met.
 */
bool BattleField::isVictory(void) const
{
    for (Mob* m : _hostiles)
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
    for (Mob* m : _pcs)
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
    for (Mob* item : mobs())
    {
        // Now is the time to clean up dead mobs.
        if (item->stamina() <= 0)
        {
            for (int i = 0; i < int(_hostiles.size()); i++)
                if (item == _hostiles.at(i))
                    _hostiles.erase(_hostiles.begin() + i);
            for (int i = 0; i < int(_pcs.size()); i++)
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

bool BattleField::areAllied(const MapObject* one, const MapObject* other) const
{
    Mob* mOne = (Mob*) one;
    Mob* mOther = (Mob*) other;
    return mOne->type() == mOther->type();
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

std::vector<Mob*> BattleField::mobs(void)
{
    std::vector<Mob*> result = std::vector<Mob*>();
    result.insert(result.end(), _hostiles.begin(), _hostiles.end());
    result.insert(result.end(), _pcs.begin(), _pcs.end());
    return result;
}
