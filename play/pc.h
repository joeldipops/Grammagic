#ifndef PC_H_INCLUDED
#define PC_H_INCLUDED

#include "mob.h"
#include "../res/battleCommands.h"
#include "../res/templates.h"

// TODO - BWAARRPP
class JobClass
{
    public:
        std::string name(void) const
        {
            return "Class";
        };
};

class PC : public Mob
{
    public:
        PC(const Templates::PCTemplate&);
        ~PC(void);
        int spellSlots(void) const;
        int runeSlots(void) const;
        std::string name(void) const;
        std::string className(void) const;
        bool isPlayerControlled(void) const;
        Templates::PartyMemberCode memberCode(void) const;


    private:
        int _spellSlots = 7;
        int _runeSlots = 6;
        std::string _name;
        Templates::PartyMemberCode _memberCode;
        JobClass _jobClass;
};
#endif
