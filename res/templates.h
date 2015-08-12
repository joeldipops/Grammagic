#ifndef PARTY_MEMBERS_H
#define PARTY_MEMBERS_H

#include <string>
#include "../globalConstants.h"

namespace Templates
{
    /**
     * The default / initial properties and stats of a mob
     */
    struct MobTemplate
    {
        std::string ImagePath;
        std::string PortraitPath;
        int Stamina;
        float Speed;
        float Resistance;
        float Defence;
        float Skill;
        unsigned int RangeOfSense;
        unsigned int RangeOfSight;
    };

    struct PCTemplate : public MobTemplate
    {
        std::string Name;
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

