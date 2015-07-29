#ifndef PC_H_INCLUDED
#define PC_H_INCLUDED

#include "mob.h"
#include "../magic/battleCommands.h"

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
        PC(void);
        int spellSlots(void) const;
        int runeSlots(void) const;
        std::string name(void) const;
        std::string className(void) const;

    private:
        int _spellSlots;
        int _runeSlots;
        std::string _name;
        JobClass _jobClass;
};
#endif
