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
BattleField::BattleField(GameMap* map_)
{
    // All party members will be in the combat.
    for (Mob* c : map_->party()->members())
    {
        _pcs.push_back(c);
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
    while ((!startChecking || indexY != startY) && partyIndex < _pcs.size())
    {
        int x = partyX + indexX;
        int y = partyY + indexY;

        MapCell* cell = map_->getCell(x, y);
        if (cell != nullptr && !cell->terrain()->isDense())
            if (cell->contents() == nullptr)
            {
                map_->place(_pcs.at(partyIndex), x, y);
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
            _hostiles.push_back(m);
            continue;
        }

        // If a mob can see any party member, that mob should be in the combat.
        for (const PC* pc : map_->party()->members())
        {
            if (m->isSeen(*pc))
            {
                _hostiles.push_back(m);
                break;
            }
        }
    }

    std::vector<void*> _rubbishBin = std::vector<void*>(0);
    std::vector<Combatable*> _playerAllied(0);
    std::vector<Combatable*> _nonPlayerAllied(0);
    _map = map_;
}

/**
 * Garbage collects any memory that was allocated during combat.
 */
BattleField::~BattleField(void)
{
    _playerAllied = std::vector<Combatable*>(0);
    _nonPlayerAllied = std::vector<Combatable*>(0);
    _map = nullptr;

    for(Combatable* item : _rubbishBin)
    {
        delete item;
    }
    _rubbishBin = std::vector<Combatable*>(0);
}

//}

//{ Properties
/**
 * Gets or sets the status of the battlefield.
 */
bool BattleField::isInCombat(void) const { return _isInCombat; }
bool BattleField::isInCombat(bool isInCombat_)
{
    _isInCombat = isInCombat_;
    return _isInCombat;
}

/**
 * List of mobs in the combat who are hostile to the player.
 */
std::vector<Mob*> BattleField::hostiles(void) { return _hostiles; }

/**
 * List of mobs who are controlled by the player.
 */
std::vector<Mob*> BattleField::pcs(void) { return _pcs; }

//}

//{ Methods

/**
 * Cleans up after a battle.
 */
void BattleField::endCombat(void)
{
    _isInCombat = false;
    _map->party()->endCombat();

    for (Combatable* mob : _hostiles)
        mob->endCombat();

    for (Mob* pc : _pcs)
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
 * Gets a list of combatable mobs who are able to perform an action this iteration.
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
            natural i;
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

/**
 * Returns true if two mobs are on the same side.
 * @param one One of two mobs.
 * @param other Two of two mobs.
 */
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

    if (found == 2)
        return true;

    for (Combatable* mob : _nonPlayerAllied)
    {
        if (mob == one)
            found++;
        if (mob == other)
            found++;
    }

    return found % 2 == 0;
}

/**
 * List of all combatable mobs and anything else participating in the battle.
 */
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
void BattleField::addToField(Combatable* mob, bool isPlayerAllied)
{
    _rubbishBin.push_back(mob);

    if (isPlayerAllied)
        _playerAllied.push_back(mob);
    else
        _nonPlayerAllied.push_back(mob);
}

//}
