#ifndef PARTY_MEMBERS_H
#define PARTY_MEMBERS_H

#include <string>
#include "../globalConstants.h"
#include "strings.h"
#include "../res/templates.h"

namespace Templates
{
    /**
     * The default / initial properties and stats of a mob
     */
    struct MobTemplate
    {
        std::string ImagePath;
        std::string PortraitPath;
        short Stamina;
        float Speed;
        float Resistance;
        float Defence;
        float Skill;
        unsigned int RangeOfSense;
        unsigned int RangeOfSight;
    };

    enum PartyMemberCode
    {
        A, B, C, D, E, F
    };

    struct PCTemplate : public MobTemplate
    {
        std::string Name;
        PartyMemberCode MemberCode;
    };

    struct EnemyTemplate : public MobTemplate
    {
        float Attack;
    };

    struct Data
    {
        public:
            static const PCTemplate A;
            static const PCTemplate B;
            static const PCTemplate C;
    };

}

#endif

