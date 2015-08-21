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
    class MapObject;
    struct PlayStateContainer;

    typedef int (*AiAction)(Mob* context, BattleField* field);
    typedef PlayStateContainer& (*PlayEventHandler)(MapObject* context, PlayStateContainer&);
}

namespace Templates
{
    struct Commands;

    /**
     * The default/initial properties and handlers of any map object
     */
    struct MapObjectTemplate
    {
        std::string ImagePath;
        bool IsDense;
        Play::PlayEventHandler OnInspect;
    };

    /**
     * The default / initial properties and stats of a mob
     */
    struct MobTemplate : public MapObjectTemplate
    {
        std::string PortraitPath;
        short Stamina;
        float Speed;
        float Resistance;
        float Defence;
        float Skill;
        natural RangeOfSense;
        natural RangeOfSight;
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

    struct TerrainTemplate : public MapObjectTemplate
    {
        Play::PlayEventHandler OnEnter;
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

            static const TerrainTemplate Grass;
            static const TerrainTemplate Wall;
            static const TerrainTemplate Hut;

            // Stats of Enemies
            static const EnemyTemplate E1;
            static const EnemyTemplate E2;

            // Stats of Bosses
            static const EnemyTemplate B1;
    };

}

#endif

