#include "battleField.h"

// Forward references
#include "mob.h"
#include "gameMap.h"

using namespace Play;

//{ Lifecycle
/**
 * Constructor
 * @param map The battlefield is essentially formed out of a subset of the map.
 */
BattleField::BattleField(GameMap* map_) : SpellContext()
{
    // All party members will be in the combat.
    for (Mob* c : map_->party()->members())
    {
        pcs().push_back(c);
    }

    // Place the rest of the party on the map in random positions around the  party leader.
    int startX = rand() % 3 - 1;
    int startY = rand() % 3 - 1;

    int indexX = startX;
    int indexY = startY;
    bool startChecking = false;
    natural partyIndex = 1;

    int partyX = map_->party()->x();
    int partyY = map_->party()->y();
    auto c = map_->contents();
    while ((!startChecking || indexY != startY) && partyIndex < pcs().size())
    {
        int x = partyX + indexX;
        int y = partyY + indexY;

        MapCell* cell = map_->getCell(x, y);
        if (cell != nullptr && !cell->terrain()->isDense())
            if (cell->contents() == nullptr)
            {
                map_->place(pcs().at(partyIndex), x, y);
                partyIndex++;
            }

        (indexX == 1) ? indexX = -1 : indexX++;
        if (indexX == startX)
        {
            (indexY == 1) ? indexY = -1 : indexY++;
            startChecking = true;
        }
    }

    // Any enemies that can see any member of the party should be in the combat.
    for(natural i = 1; i < map_->contents().size(); i++)
    {
        if (!map_->contents().at(i)->isMob())
            continue;
        Mob* m = (Mob*) map_->contents().at(i);

        if (m->isPlayerControlled())
            continue;

        if (m->isInCombat())
        {
            hostiles().push_back(m);
            continue;
        }

        // If a mob can see any party member, that mob should be in the combat.
        for (const PC* pc : map_->party()->members())
        {
            if (m->isSeen(*pc))
            {
                hostiles().push_back(m);
                break;
            }
        }
    }

    std::vector<void*> _rubbishBin = std::vector<void*>(0);
    std::vector<Combatable*> _playerAllied(0);
    std::vector<Combatable*> _nonPlayerAllied(0);
    _map = map_;
}

BattleField::~BattleField(void)
{
    _map = nullptr;
}

//}

//{ Properties


//}

//{ Methods

/**
 *
 */
 void BattleField::buryTheDead(void)
 {
    for (natural i = 0; i < hostiles().size(); i++)
    {
        if (hostiles().at(i)->stamina() <= 0)
        {
            hostiles().erase(hostiles().begin() + i);
            i--;
        }
    }
    _map->buryTheDead();
 }

/**
 * Cleans up after a battle.
 */
void BattleField::endCombat(void)
{
    isInCombat(false);
    _map->party()->endCombat();

    for (Combatable* mob : hostiles())
        mob->endCombat();

    for (Mob* pc : pcs())
    {
        pc->endCombat();
        // Remove from the map.
        if (pc != _map->party()->leader())
            _map->remove(pc);
    }
}

/**
 * Return true if a victory condition (for the player) has been met.
 */
bool BattleField::isVictory(void) const
{
    for (Combatable* m : hostiles())
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
    for (Combatable* m : pcs())
    {
        if (m->stamina() > 0)
            return false;
    }

    return true;
}

/**
 * Gets a list of combatable mobs who are able to perform an action this iteration.
 */
std::vector<Mob*> BattleField::getDue(void)
{
    std::vector<Mob*> result = std::vector<Mob*>(0);
    if (!isInCombat())
        return result;

    int time = SDL_GetTicks();
    for (Combatable* c : participants())
    {
        Mob* item = (Mob*) c;
        // Now is the time to clean up dead mobs.
        if (item->stamina() <= 0)
        {
            natural i;
            for (i = 0; i < hostiles().size(); i++)
                if (item == hostiles().at(i))
                    hostiles().erase(hostiles().begin() + i);
            for (i = 0; i < pcs().size(); i++)
                if (item == pcs().at(i))
                    pcs().erase(pcs().begin() + i);
            continue;
        }

        if (item->tryUnblock(time) || !item->isBlocked())
            result.push_back(item);
    }
    result.shrink_to_fit();
    return result;
}

//}
