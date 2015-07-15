#ifndef PC_H_INCLUDED
#define PC_H_INCLUDED

#include "mob.h"
#include "../magic/battleCommands.h"

class PC : public Mob
{
    public:
        PC(void);
        int spellSlots(void) const;
        int runeSlots(void) const;

    private:
        int _spellSlots;
        int _runeSlots;
};
#endif
