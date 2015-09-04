#ifndef PC_H_INCLUDED
#define PC_H_INCLUDED

#include "mob.h"
#include "../res/battleCommands.h"
#include "../res/templates.h"
#include "jobClass.h"

class PC : public Mob
{
    public:
        PC(const Templates::PCTemplate&);
        ~PC(void);
        natural spellSlots(void) const;
        natural runeSlots(void) const;
        const std::string& name(void) const;
        const std::string& className(void) const;
        bool isPlayerControlled(void) const;
        Templates::PartyMemberCode memberCode(void) const;


    private:
        natural _spellSlots = 7;
        natural _runeSlots = 6;
        std::string _name;
        Templates::PartyMemberCode _memberCode;
        JobClass _jobClass;
};
#endif
