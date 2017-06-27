#ifndef TEMPLATES_H
#define TEMPLATES_H

#include <string>
#include <vector>

#include "../util/events.h"

#include "../globalConstants.h"
#include "../magic/magicTypedefs.h"

namespace Magic { class Command; class SpellContext; }
namespace Play
{
    class Mob;
    class BattleField;
    class MapObject;
    struct PlayStateContainer;

    typedef int (*AiAction)(Mob* context, Magic::SpellContext& field);
    typedef PlayStateContainer& (*PlayEventHandler)(MapObject* context, PlayStateContainer&);
}

namespace Resources
{
    struct Commands;

    struct RuneTemplate
    {
        std::string Name = "";
        std::string ImagePath = "";

        int AddEffect = 0;
        int AddCost = 0;
        int AddDuration = 0;

        float ModEffect = 1;
        float ModCost = 1;
        float ModDuration = 1;

        Magic::MetaAction MetaAction = nullptr;
        Magic::Modality Flag = Magic::Modality::NA;

        Magic::Targeter GetTarget = nullptr;
        Magic::MultiTargeter GetTargetCandidates = nullptr;
        Magic::Selecter SelectTargetFromCandidates = nullptr;

        Magic::Action PerformAction = nullptr;
        float IsSameMultiplier = 1;
        float EnemyCostMultiplier = 1;
        float AllyEffectMultiplier = 1;
        int ActionEffectBonus = 0;
        int ActionCostBonus = 0;
        int ActionDurationBonus = 0;
        bool IsBoon = false;

        Persistence::SavedObjectCode Code = Persistence::SavedObjectCode::UNKNOWN;
    };

    struct JobTemplate
    {
        std::string Name;
        float StaminaGrowth;
        float SkillGrowth;
        float SpeedGrowth;
        float ResistanceGrowth;
        float DefenceGrowth;
        Persistence::SavedObjectCode Code;
    };

    /**
     * The default/initial properties and handlers of any map object
     */
    struct MapObjectTemplate
    {
        std::string ImagePath;
        bool IsDense;
        Handler<MapObject, PlayStateContainer> OnInspect = nullptr;
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
        JobTemplate Class;
    };

    struct EnemyTemplate : public MobTemplate
    {
        float Attack;
        int AttackDelay;
        int MovementDelay;
        int Reward;
        Play::AiAction CombatAction = nullptr;
    };

    struct TerrainTemplate : public MapObjectTemplate
    {
        Play::PlayEventHandler OnEnter = nullptr;
    };

    struct Data
    {
        public:
            static const JobTemplate TANK;
            static const JobTemplate WELLSPRING;
            static const JobTemplate GLASSCANON;
            static const JobTemplate JACK;
            static const JobTemplate GUARD;
            static const JobTemplate WASP;

            // Starting Stats of Playable Characters.
            static const PCTemplate A;
            static const PCTemplate B;
            static const PCTemplate C;
            static const PCTemplate D;
            static const PCTemplate E;
            static const PCTemplate F;

            static const EnemyTemplate NPC1;

            static const TerrainTemplate Grass;
            static const TerrainTemplate Wall;
            static const TerrainTemplate Hut;
            static const TerrainTemplate Cache;

            // Stats of Enemies
            static const EnemyTemplate E1;
            static const EnemyTemplate E2;

            // Stats of Bosses
            static const EnemyTemplate B1;

            // All Runes
            static const RuneTemplate ALL;
            static const RuneTemplate ANY;
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

