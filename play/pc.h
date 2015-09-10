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
        const Persistence::SavedObjectCode classCode(void) const;
        bool isPlayerControlled(void) const;
        Templates::PartyMemberCode memberCode(void) const;
        void endCombat(void);

        void getSpoils(int);

    protected:
        static const natural STAT_INCREASE_THRESHOLD = 30;

    private:
        void applySpoils(void);

        natural _spellSlots = 7;
        natural _runeSlots = 6;
        std::string _name;
        Templates::PartyMemberCode _memberCode;
        JobClass _jobClass;

        natural _staminaEXP = 0;
        natural _skillEXP = 0;
        natural _speedEXP = 0;
        natural _defenceEXP = 0;
        natural _resistanceEXP = 0;
};
#endif
