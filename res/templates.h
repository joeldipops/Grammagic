#ifndef PARTY_MEMBERS_H
#define PARTY_MEMBERS_H

#include <string>
#include <vector>

#include "strings.h"

#include "../globalConstants.h"

namespace Magic { class Command; }
namespace Play
{
    class Mob;
    class BattleField;
    typedef int (*AiAction)(Mob* actor, BattleField* field);
}

namespace Templates
{
    struct Commands;
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
        std::vector<Magic::Command*> Commands;
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
        int AttackDelay;
        int MovementDelay;
        Play::AiAction CombatAction;
    };

    struct Data
    {
        public:
            // Starting Stats of Playable Characters.
            static const PCTemplate A;
            static const PCTemplate B;
            static const PCTemplate C;
            static const PCTemplate D;
            static const PCTemplate E;
            static const PCTemplate F;

            // Stats of Enemies
            static const EnemyTemplate E1;
            static const EnemyTemplate E2;

            // Stats of Bosses
            static const EnemyTemplate B1;
    };

}

#endif

