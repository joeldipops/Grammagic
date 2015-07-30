#include "party.h"

using namespace Play;

/**
 * Constructor
 */
Party::Party(void)
    :MapObject()
{
    _members = std::vector<PC*>(0);
    imageFileName(RESOURCE_LOCATION + "pc.png");
    isDense(true);
    x(0);
    y(0);
}

/**
 * Constructor takes list of members.
 */
Party::Party(std::vector<PC*> members_)
    :MapObject()
{
    _members = members_;
}

/**
 * Destructor
 */
Party::~Party(void)
{
    for(unsigned int i = 0; i < _members.size(); i++)
    {
        delete _members.at(i);
    }

    _members = std::vector<PC*>(0);
}

const std::vector<PC*> Party::members(void) const
{
    return _members;
}

PC* Party::leader(void) const
{
    if (_members.size() >= 1)
        return _members.at(0);

    return nullptr;
}

PC* Party::addLeader(void)
{
    PC* pc = new PC();
    if (_members.size() >= 1)
    {
        delete _members.at(0);
        _members[0] = pc;
    }
    else
        _members.push_back(pc);

    pc->location(x(), y());
    return pc;
}


PC* Party::addMember(void)
{
    if (_members.size() <= 0)
        return addLeader();

    PC* pc = new PC();
    _members.push_back(pc);
    return pc;
}

/**
 * Finds the position of a specific pc in the party.
 * pc The pc to look for.
 */
 int Party::getIndexOf(const Mob* pc) const
{
    for(unsigned int i = 0; i < _members.size(); i++)
    {
        if (pc == _members.at(i))
            return i;
    }
    return -1;
}

bool Party::isPlayerParty(void) const
{
    return true;
}

/**
 * Party is defeated if all members are out of stamina.
 */
bool Party::isDefeated(void) const
{
    for(PC* pc : _members)
    {
        if (pc->stamina() > 0)
            return false;
    }
    return true;
}

int Party::x(int x_)
{
    if (_members.size() > 0)
        leader()->x(x_);
    return MapObject::x(x_);
}

int Party::y(int y_)
{
    if (_members.size() > 0)
        leader()->y(y_);
    return MapObject::y(y_);
}
