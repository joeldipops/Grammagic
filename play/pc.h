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

        void getSpoils(int);

    protected:
        static const natural STAT_INCREASE_THRESHOLD = 100;

    private:
        void applySpoils(void);
        void applySpoilForStat(int&, natural&);

        natural _spellSlots = 7;
        natural _runeSlots = 6;
        std::string _name;
        Templates::PartyMemberCode _memberCode;
        JobClass _jobClass;

        natural _staminaEXP;
        natural _skillEXP;
        natural _speedEXP;
        natural _defenceEXP;
        natural _resistanceEXP;
};
#endif
