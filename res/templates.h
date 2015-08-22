#ifndef TEMPLATES_H
#define TEMPLATES_H

#include <string>
#include <vector>

#include "strings.h"

#include "../globalConstants.h"
#include "../magic/magicTypedefs.h"

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

    struct RuneTemplate
    {
        std::string Name;
        std::string ImagePath;
        Magic::Targeter GetTarget;
        Magic::MultiTargeter GetMultipleTarget;
        Magic::Selecter SelectTarget;
        Magic::Action PerformAction;
        Stat RelatedStat;

        int AddEffect;
        int AddCost;
        int AddDuration;

        float ModEffect;
        float ModCost;
        float ModDuration;
    };

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

            // All Runes
            static const RuneTemplate ALL;
            static const RuneTemplate CASTER;
            static const RuneTemplate ENEMY;
            static const RuneTemplate ALLY; // Excludes the caster.
            static const RuneTemplate MEMBER; // Includes the caster.
            static const RuneTemplate HIGH;
            static const RuneTemplate LOW;
            static const RuneTemplate STAMINA;
            static const RuneTemplate SPEED;
            static const RuneTemplate DEFENCE;
            static const RuneTemplate RESISTANCE;
            static const RuneTemplate SKILL;
            static const RuneTemplate STRENGTH;
            static const RuneTemplate SPELL;
    };

}

#endif

