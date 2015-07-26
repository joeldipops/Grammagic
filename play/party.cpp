#include "party.h"

using namespace Play;

Party::Party(void)
    :MapObject()
{
    _members = std::vector<PC*>(0);
    addLeader();
    imageFileName(RESOURCE_LOCATION + "pc.png");
    isDense(true);
}

Party::Party(std::vector<PC*> members_)
    :MapObject()
{
    _members = members_;
}

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
    return pc;
}



PC* Party::addMember(void)
{
    PC* pc = new PC();
    _members.push_back(pc);
    return pc;
}

bool Party::isPlayerParty(void) const
{
    return true;
}

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
    leader()->x(x_);
    return MapObject::x(x_);
}

int Party::y(int y_)
{
    leader()->y(y_);
    return MapObject::y(y_);
}
