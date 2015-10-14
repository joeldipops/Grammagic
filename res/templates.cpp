#include "templates.h"
#include "battleCommands.h"
#include "strings.h"
#include "../play/playStateManager.h"
using namespace Resources;
using namespace Persistence;

//{ Classes
const JobTemplate getTANK()
{
    JobTemplate result = JobTemplate();
    result.Name = Strings::Tank;
    result.Code = SavedObjectCode::TANK_CLASS;
    result.StaminaGrowth = 2.6;
    result.DefenceGrowth = 2.0;
    result.ResistanceGrowth = 2.0;
    result.SpeedGrowth = 1.5;
    result.SkillGrowth = 1.5;
    return result;
}
const JobTemplate Data::TANK = getTANK();

const JobTemplate getWELLSPRING()
{
    JobTemplate result = JobTemplate();
    result.Name = Strings::WellSpring;
    result.Code = SavedObjectCode::WELLSPRING_CLASS;
    result.StaminaGrowth = 3.4;
    result.DefenceGrowth = 1.8;
    result.ResistanceGrowth = 1.8;
    result.SpeedGrowth = 1.0;
    result.SkillGrowth = 1.0;
    return result;
}
const JobTemplate Data::WELLSPRING = getWELLSPRING();

const JobTemplate getGLASSCANON()
{
    JobTemplate result = JobTemplate();
    result.Name = Strings::GlassCannon;
    result.Code = SavedObjectCode::GLASSCANNON_CLASS;
    result.StaminaGrowth = 1.6;
    result.DefenceGrowth = 1.2;
    result.ResistanceGrowth = 1.2;
    result.SpeedGrowth = 2.5;
    result.SkillGrowth = 3.0;
    return result;
}
const JobTemplate Data::GLASSCANON = getGLASSCANON();

const JobTemplate getJACK()
{
    JobTemplate result = JobTemplate();
    result.Name = Strings::Jack;
    result.Code = SavedObjectCode::JACK_CLASS;
    result.StaminaGrowth = 2.0;
    result.DefenceGrowth = 2.0;
    result.ResistanceGrowth = 2.0;
    result.SpeedGrowth = 2.0;
    result.SkillGrowth = 2.0;
    return result;
}
const JobTemplate Data::JACK = getJACK();

const JobTemplate getGUARD()
{
    JobTemplate result = JobTemplate();
    result.Name = Strings::Guard;
    result.Code = SavedObjectCode::GUARD_CLASS;
    result.StaminaGrowth = 1.8;
    result.DefenceGrowth = 2.4;
    result.ResistanceGrowth = 2.4;
    result.SpeedGrowth = 1.0;
    result.SkillGrowth = 2.4;
    return result;
}
const JobTemplate Data::GUARD = getGUARD();

const JobTemplate getWASP()
{
    JobTemplate result = JobTemplate();
    result.Name = Strings::Wasp;
    result.Code = SavedObjectCode::WASP_CLASS;
    result.StaminaGrowth = 1.7;
    result.DefenceGrowth = 1.5;
    result.ResistanceGrowth = 1.5;
    result.SpeedGrowth = 3.3;
    result.SkillGrowth = 2.0;
    return result;
}
const JobTemplate Data::WASP = getWASP();
//}

//{Runes

static Combatable* all(Mob* caster, SpellContext& field, const std::vector<Combatable*>& candidates, SpellData&)
{
    TargetAll* result = new TargetAll(candidates);

    bool isPlayerAllied = field.areAllied(caster, field.pcs().at(0));

    // Should the target be considered an ally or an of the caster?
    natural allyCount = 0;
    for (const Combatable* c : candidates)
    {
        if (field.areAllied(caster, c))
            allyCount++;
    }


    if (allyCount > candidates.size() / 2.0)
        field.addToField(result, isPlayerAllied);
    else
        field.addToField(result, !isPlayerAllied);

    return result;
}
const RuneTemplate GetALL()
{
    RuneTemplate result = RuneTemplate();
    result.Name = "ALL";
    result.Code = Persistence::SavedObjectCode::ALL_RUNE;
    result.ImagePath = RESOURCE_LOCATION + "all.png";
    result.SelectTargetFromCandidates = all;

    result.AddEffect = 0;
    result.AddCost = 0;
    result.AddDuration = 0;

    result.ModEffect = 1.0;
    result.ModCost = 1.0;
    result.ModDuration = 1.0;
    return result;
};
const RuneTemplate Resources::Data::ALL = GetALL();

static Combatable* any (Mob* caster, SpellContext& field, const std::vector<Combatable*>& candidates, SpellData&)
{
    int index  = rand() % candidates.size();
    return candidates.at(index);
}
const RuneTemplate GetANY()
{
    RuneTemplate result = RuneTemplate();
    result.Name = "ANY";
    result.Code = Persistence::SavedObjectCode::ANY_RUNE;
    result.ImagePath = RESOURCE_LOCATION + "any.png";
    result.SelectTargetFromCandidates = any;

    result.AddEffect = 0;
    result.AddCost = 0;
    result.AddDuration = 0;

    result.ModEffect = 1.0;
    result.ModCost = 1.0;
    result.ModDuration = 1.0;
    return result;
};
const RuneTemplate Resources::Data::ANY = GetANY();


static Combatable* self (Mob* caster, SpellContext&, SpellData&)
{
    return (Combatable*) caster;
};
const RuneTemplate GetCASTER()
{
    RuneTemplate result = RuneTemplate();
    result.Name = "CASTER";
    result.Code = Persistence::SavedObjectCode::CASTER_RUNE;
    result.ImagePath = RESOURCE_LOCATION + "caster.png";
    result.GetTarget = self;

    result.AddEffect = 0;
    result.AddCost = 0;
    result.AddDuration = 0;

    result.ModEffect = 1.0;
    result.ModCost = 1.0;
    result.ModDuration = 1.0;
    return result;
};
const RuneTemplate Resources::Data::CASTER = GetCASTER();


static std::vector<Combatable*> enemies(Mob* caster, SpellContext& battleField, SpellData&)
{
    std::vector<Combatable*> result;
    std::vector<Mob*> candidates;

    if (caster->type() == MobType::PlayerCharacter)
        candidates =  battleField.hostiles();
    else
        candidates = battleField.pcs();

    for (Mob* m : candidates)
    {
        result.push_back(m);
    }

    return result;
};
const RuneTemplate GetENEMY()
{
    RuneTemplate result = RuneTemplate();
    result.Name = "ENEMY";
    result.Code = Persistence::SavedObjectCode::ENEMY_RUNE;
    result.ImagePath = RESOURCE_LOCATION + "enemy.png";
    result.GetTargetCandidates = enemies;

    result.AddEffect = 0;
    result.AddCost = 0;
    result.AddDuration = 0;

    result.ModEffect = 1.0;
    result.ModCost = 1.0;
    result.ModDuration = 1.0;
    return result;
};
const RuneTemplate Resources::Data::ENEMY = GetENEMY();

static std::vector<Combatable*> allies(Mob* caster, SpellContext& battleField, SpellData&)
{
    std::vector<Combatable*> result;
    std::vector<Mob*> candidates;

    if (caster->type() == MobType::PlayerCharacter)
        candidates = battleField.pcs();
    else
        candidates =  battleField.hostiles();

    for (Mob* m : candidates)
    {
        if (m != caster)
            result.push_back(m);
    }

    return result;
};
const RuneTemplate GetALLY()
{
    RuneTemplate result = RuneTemplate();
    result.Name = "ALLY";
    result.Code = Persistence::SavedObjectCode::ALLY_RUNE;
    result.ImagePath = RESOURCE_LOCATION + "ally.png";
    result.GetTargetCandidates = allies;

    result.AddEffect = 0;
    result.AddCost = 0;
    result.AddDuration = 0;

    result.ModEffect = 1.0;
    result.ModCost = 1.0;
    result.ModDuration = 1.0;
    return result;
};
const RuneTemplate Resources::Data::ALLY = GetALLY();

static std::vector<Combatable*> members(Mob* caster, SpellContext& battleField, SpellData& data)
{
    std::vector<Combatable*> result;
    std::vector<Mob*> candidates;

    if (caster->type() == MobType::PlayerCharacter)
        candidates = battleField.pcs();
    else
        candidates =  battleField.hostiles();

    for (Mob* m : candidates)
    {
        result.push_back(m);
    }

    return result;
};

const RuneTemplate GetMEMBER()
{
    RuneTemplate result = RuneTemplate();
    result.Name = "MEMBER";
    result.Code = Persistence::SavedObjectCode::MEMBER_RUNE;
    result.ImagePath = RESOURCE_LOCATION + "member.png";
    result.GetTargetCandidates = members;

    result.AddEffect = 0;
    result.AddCost = 0;
    result.AddDuration = 0;

    result.ModEffect = 1.0;
    result.ModCost = 1.0;
    result.ModDuration = 1.0;
    return result;
};
const RuneTemplate Resources::Data::MEMBER = GetMEMBER();

SpellData& modifySpell(SpellData& data, float metaCost, float metaEffect, float metaDuration)
{
    data.cost *= 1 + metaCost;
    data.duration *= 1 + metaEffect;
    data.effect *= 1 + metaDuration;
    return  data;
}

Combatable* most(Mob* caster, SpellContext& battleField, const std::vector<Combatable*>& candidates, SpellData& data)
{
    Combatable* result = nullptr;
    for(natural i = 0; i < candidates.size(); i++)
    {
        Combatable* mob = candidates.at(i);

        if (result == nullptr)
            result = mob;
        else
        {
            float resultValue;
            float mobValue;
            switch(data.stat)
            {
                case Stat::SKILL:
                    resultValue = result->skill();
                    mobValue = mob->skill();
                    break;
                case Stat::DEFENSE:
                    resultValue = result->defence();
                    mobValue = mob->defence();
                    break;
                case Stat::SPEED:
                    resultValue = result->speed();
                    mobValue = mob->speed();
                    break;
                case Stat::RESISTANCE:
                    resultValue = result->resistance();
                    mobValue = mob->resistance();
                    break;
                case Stat::STAMINA:
                    resultValue = result->stamina();
                    mobValue = mob->stamina();
                    break;
                default: break;
            }
            if (data.modality == Modality::HIGH)
            {
                if (mobValue > resultValue)
                    result = mob;
            }
            else
            {
                if (mobValue < resultValue)
                    result = mob;
            }
        }
    }

    return result;
};

void changeStat(Combatable* source, Combatable* target, int cost, int effect, SpellData& data)
{
    if (data.modality == Modality::LOW)
        effect *= -1;

    source->changeStamina(cost * -1);
    switch(data.stat)
    {
        case Stat::STAMINA:
            target->changeStamina(effect);
            return;
        case Stat::SKILL:
            target->changeSkill(1 + (float(effect) / 100.0));
            return;
        case Stat::DEFENSE:
            target->changeDefence(1 + (float(effect) / 100.0));
            return;
        case Stat::SPEED:
            target->changeSpeed(1 + (float(effect) / 100.0));
            return;
        case Stat::RESISTANCE:
            target->changeResistance(1 + (float(effect) / 100.0));
            return;
        default:
            break;
    }
}

SpellData& heavySpell(SpellData& data)
{
    return modifySpell(data, .9, 1, .1);
}

const RuneTemplate GetHIGH()
{
    RuneTemplate result = RuneTemplate();
    result.Name = "HIGH";
    result.Code = Persistence::SavedObjectCode::HIGH_RUNE;
    result.ImagePath = RESOURCE_LOCATION + "up.png";
    result.PerformAction;
    result.MetaAction = heavySpell;

    result.Flag = Modality::HIGH;

    result.AddEffect = 0;
    result.AddCost = 0;
    result.AddDuration = 0;

    result.ModEffect = 1.0;
    result.ModCost = 1.0;
    result.ModDuration = 1.0;
    return result;
};
const RuneTemplate Resources::Data::HIGH = GetHIGH();

SpellData& lightSpell(SpellData& data)
{
    return modifySpell(data, -.5, -.5, 0);
}

const RuneTemplate GetLOW()
{
    RuneTemplate result = RuneTemplate();
    result.Name = "LOW";
    result.Code = Persistence::SavedObjectCode::LOW_RUNE;
    result.ImagePath = RESOURCE_LOCATION + "down.png";
    result.PerformAction;
    result.MetaAction = lightSpell;

    result.Flag = Modality::LOW;

    result.AddEffect = 0;
    result.AddCost = 0;
    result.AddDuration = 0;

    result.ModEffect = 1.0;
    result.ModCost = 1.0;
    result.ModDuration = 1.0;
    return result;
};
const RuneTemplate Resources::Data::LOW = GetLOW();


Combatable* mostStamina(Mob* caster, SpellContext& battleField, const std::vector<Combatable*>& candidates, SpellData& data)
{
    data.stat = Stat::STAMINA;
    return most(caster, battleField, candidates, data);
}

void changeStamina(Combatable* source, Combatable* target, int cost, int effect, SpellData& data)
{
    data.stat = Stat::STAMINA;
    changeStat(source, target, cost, effect, data);
}

const RuneTemplate GetSTAMINA()
{
    RuneTemplate result = RuneTemplate();
    result.Name = "STAMINA";
    result.Code = Persistence::SavedObjectCode::STAMINA_RUNE;
    result.ImagePath = RESOURCE_LOCATION + "stamina.png";
    result.SelectTargetFromCandidates = mostStamina;
    result.PerformAction = changeStamina;

    result.AddEffect = 0;
    result.AddCost = 0;
    result.AddDuration = 0;

    result.ModEffect = 1.0;
    result.ModCost = 1.0;
    result.ModDuration = 1.0;

    result.IsBoon = true;
    result.ActionCostBonus = 15;
    result.ActionEffectBonus = 30;
    result.ActionDurationBonus = 3000;
    return result;
};
const RuneTemplate Resources::Data::STAMINA = GetSTAMINA();

SpellData& fastSpell(SpellData& data)
{
    return modifySpell(data, 0, 0, -.25);
}

Combatable* mostSpeed(Mob* caster, SpellContext& battleField, const std::vector<Combatable*>& candidates, SpellData& data)
{
    data.stat = Stat::SPEED;
    return most(caster, battleField, candidates, data);
}

void changeSpeed(Combatable* source, Combatable* target, int cost, int effect, SpellData& data)
{
    data.stat = Stat::SPEED;
    return changeStat(source, target, cost, effect, data);
}

const RuneTemplate GetSPEED()
{
    RuneTemplate result = RuneTemplate();
    result.Name = "SPEED";
    result.Code = Persistence::SavedObjectCode::SPEED_RUNE;
    result.ImagePath = RESOURCE_LOCATION + "speed.png";
    result.SelectTargetFromCandidates = mostSpeed;
    result.PerformAction = changeSpeed;
    result.MetaAction = fastSpell;

    result.AddEffect = 0;
    result.AddCost = 0;
    result.AddDuration = 0;

    result.ModEffect = 1.0;
    result.ModCost = 1.0;
    result.ModDuration = 1.0;

    result.IsBoon = true;
    result.ActionCostBonus = 15;
    result.ActionEffectBonus = 30;
    result.ActionDurationBonus = 3000;
    return result;
};
const RuneTemplate Resources::Data::SPEED = GetSPEED();

Combatable* mostDefence(Mob* caster, SpellContext& battleField, const std::vector<Combatable*>& candidates, SpellData& data)
{
    data.stat = Stat::DEFENSE;
    return most(caster, battleField, candidates, data);
}

void changeDefence(Combatable* source, Combatable* target, int cost, int effect, SpellData& data)
{
    data.stat = Stat::DEFENSE;
    return changeStat(source, target, cost, effect, data);
}

const RuneTemplate GetDEFENCE()
{
    RuneTemplate result = RuneTemplate();
    result.Name = "DEFENCE";
    result.Code = Persistence::SavedObjectCode::DEFENCE_RUNE;
    result.ImagePath = RESOURCE_LOCATION + "defence.png";
    result.SelectTargetFromCandidates = mostDefence;
    result.PerformAction = changeDefence;

    result.AddEffect = 0;
    result.AddCost = 0;
    result.AddDuration = 0;

    result.ModEffect = 1.0;
    result.ModCost = 1.0;
    result.ModDuration = 1.0;

    result.IsBoon = true;
    result.ActionCostBonus = 15;
    result.ActionEffectBonus = 30;
    result.ActionDurationBonus = 3000;
    return result;
};
const RuneTemplate Resources::Data::DEFENCE = GetDEFENCE();

Combatable* mostResistance(Mob* caster, SpellContext& battleField, const std::vector<Combatable*>& candidates, SpellData& data)
{
    data.stat = Stat::RESISTANCE;
    return most(caster, battleField, candidates, data);
}

void changeResistance(Combatable* source, Combatable* target, int cost, int effect, SpellData& data)
{
    data.stat = Stat::RESISTANCE;
    return changeStat(source, target, cost, effect, data);
}

const RuneTemplate GetRESISTANCE()
{
    RuneTemplate result = RuneTemplate();
    result.Name = "RESISTANCE";
    result.Code = Persistence::SavedObjectCode::RESISTANCE_RUNE;
    result.ImagePath = RESOURCE_LOCATION + "resistance.png";
    result.SelectTargetFromCandidates = mostResistance;
    result.PerformAction = changeResistance;

    result.AddEffect = 0;
    result.AddCost = 0;
    result.AddDuration = 0;

    result.ModEffect = 1.0;
    result.ModCost = 1.0;
    result.ModDuration = 1.0;

    result.IsBoon = true;
    result.ActionCostBonus = 15;
    result.ActionEffectBonus = 30;
    result.ActionDurationBonus = 3000;
    return result;
};
const RuneTemplate Resources::Data::RESISTANCE = GetRESISTANCE();

Combatable* mostSkill(Mob* caster, SpellContext& battleField, const std::vector<Combatable*>& candidates, SpellData& data)
{
    data.stat = Stat::SKILL;
    return most(caster, battleField, candidates, data);
}

void changeSkill(Combatable* source, Combatable* target, int cost, int effect, SpellData& data)
{
    data.stat = Stat::SKILL;
    return changeStat(source, target, cost, effect, data);
}

const RuneTemplate GetSKILL()
{
    RuneTemplate result = RuneTemplate();
    result.Name = "SKILL";
    result.Code = Persistence::SavedObjectCode::SKILL_RUNE;
    result.ImagePath = RESOURCE_LOCATION + "skill.png";
    result.SelectTargetFromCandidates = mostSkill;
    result.PerformAction = changeSkill;

    result.AddEffect = 0;
    result.AddCost = 0;
    result.AddDuration = 0;

    result.ModEffect = 1.0;
    result.ModCost = 1.0;
    result.ModDuration = 1.0;

    result.IsBoon = true;
    result.ActionCostBonus = 15;
    result.ActionEffectBonus = 30;
    result.ActionDurationBonus = 3000;
    return result;
};
const RuneTemplate Resources::Data::SKILL = GetSKILL();

//}

//{Party Members
PCTemplate GetA() // "Albert" archetype
{
    PCTemplate result;
    result.Name = Strings::AName;
    result.ImagePath = RESOURCE_LOCATION + "a-image.png";
    result.PortraitPath = RESOURCE_LOCATION + "a-portrait.png";
    result.Class = Data::WASP;
    result.Stamina = 100;
    result.Speed = 2.0;
    result.Resistance = 1.1;
    result.Defence = 0.9;
    result.Skill = 1.5;
    result.MemberCode = PartyMemberCode::A;
    return result;
};
const PCTemplate Data::A = GetA();

PCTemplate GetB() // "Brienne" Archetype
{
    PCTemplate result;
    result.Name = Strings::BName;
    result.ImagePath = RESOURCE_LOCATION + "b-image.png";
    result.PortraitPath = RESOURCE_LOCATION + "b-portrait.png";
    result.Class = Data::WELLSPRING;
    result.Stamina = 200;
    result.Speed = 0.5;
    result.Resistance = 2.0;
    result.Defence = 2.0;
    result.Skill = 0.6;
    result.MemberCode = PartyMemberCode::B;


    return result;
};
const PCTemplate Data::B = GetB();

const PCTemplate GetC() //"All Rounder" archetype
{
    PCTemplate result;
    result.Name = Strings::CName;
    result.ImagePath = RESOURCE_LOCATION + "c-image.png";
    result.PortraitPath = RESOURCE_LOCATION + "c-portrait.png";
    result.Class = Data::JACK;
    result.Stamina = 100;
    result.Speed = 1.0;
    result.Resistance = 1.0;
    result.Defence = 1.0;
    result.Skill = 1.0;
    result.MemberCode = PartyMemberCode::C;
    return result;
};
const PCTemplate Data::C = GetC();
//}


PlayStateContainer& receiveTutorial(MapObject* context, PlayStateContainer& data)
{
    data.Message = Strings::Tutorial;
    data.State = PlayState::Message;
    return data;
}
const EnemyTemplate GetNPC1()
{
    EnemyTemplate result;
    result.ImagePath = RESOURCE_LOCATION + "npc.png";
    result.MovementDelay = 3000;
    result.Stamina = 1;
    result.OnInspect = receiveTutorial;
    return result;
}
const EnemyTemplate Data::NPC1 = GetNPC1();

//{Types of Enemy
int aiAttack(Play::Mob* context, SpellContext& field)
{
    return Commands::ATTACK(nullptr, context, field);
}

const EnemyTemplate GetE1()
{
    EnemyTemplate result;
    result.CombatAction = aiAttack;
    result.ImagePath = RESOURCE_LOCATION + "e1.png";
    result.Stamina = 50;
    result.Attack = 10;
    result.Speed = 1.1;
    result.Resistance = .6;
    result.Defence = 1.0;
    result.Skill = 0;
    result.RangeOfSense = 3;
    result.RangeOfSight = 1;
    result.AttackDelay = 3000;
    result.MovementDelay = 3000;
    result.Reward = 10;
    return result;
};
const EnemyTemplate Data::E1 = GetE1();

const EnemyTemplate GetE2()
{
    EnemyTemplate result;
    result.CombatAction = aiAttack;
    result.ImagePath = RESOURCE_LOCATION + "e2.png";
    result.Stamina = 100;
    result.Attack = 8;
    result.Speed = .8;
    result.Resistance = .9;
    result.Defence = 1.0;
    result.Skill = 1.2;
    result.RangeOfSense = 0;
    result.RangeOfSight = 2;
    result.AttackDelay = 3000;
    result.MovementDelay = 3000;
    result.Reward = 20;
    return result;
};
const EnemyTemplate Data::E2 = GetE2();

const EnemyTemplate GetB1()
{
    EnemyTemplate result;
    result.CombatAction = aiAttack;
    result.ImagePath = RESOURCE_LOCATION + "b1.png";
    result.Stamina = 300;
    result.Attack = 25;
    result.Speed = 1.1;
    result.Resistance = 1.5;
    result.Defence = 2.0;
    result.Skill = 1.5;
    result.RangeOfSense = 4;
    result.RangeOfSight = 0;
    result.AttackDelay = 3000;
    result.MovementDelay = 2000;
    result.Reward = 40;
    return result;
};
const EnemyTemplate Data::B1 = GetB1();
//}

//{Types of Terrain
const TerrainTemplate GetWall()
{
    TerrainTemplate result;
    result.ImagePath = RESOURCE_LOCATION + "wall.png";
    result.IsDense = true;
    return result;
};
const TerrainTemplate Data::Wall = GetWall();

const TerrainTemplate GetGrass()
{
    TerrainTemplate result;
    result.ImagePath = RESOURCE_LOCATION + "grass.png";
    result.IsDense = false;
    return result;
};
const TerrainTemplate Data::Grass = GetGrass();

PlayStateContainer& getAllRunes(MapObject* context, PlayStateContainer& data)
{
    std::vector<Rune*> runes;
    runes.push_back(new Rune(Data::ALL));
    runes.push_back(new Rune(Data::ANY));
    runes.push_back(new Rune(Data::HIGH));
    runes.push_back(new Rune(Data::LOW));
    runes.push_back(new Rune(Data::ALLY));
    runes.push_back(new Rune(Data::MEMBER));
    runes.push_back(new Rune(Data::CASTER));
    runes.push_back(new Rune(Data::ENEMY));
    runes.push_back(new Rune(Data::DEFENCE));
    runes.push_back(new Rune(Data::RESISTANCE));
    runes.push_back(new Rune(Data::SKILL));
    runes.push_back(new Rune(Data::SPEED));
    runes.push_back(new Rune(Data::STAMINA));

    data.Map->party()->addRunes(runes);
    data.Message = "Woo, you picked up a bunch of runes!";

    MapCell cell(Data::Grass);
    data.Map->setCell(context->x(), context->y(), &cell);
    return data;
}

const TerrainTemplate GetCache()
{
    TerrainTemplate result;
    result.ImagePath = RESOURCE_LOCATION + "cache.png";
    result.IsDense = true;
    result.OnInspect = getAllRunes;
    return result;
};
const TerrainTemplate Data::Cache = GetCache();

PlayStateContainer& healAtHut(MapObject* context, PlayStateContainer& data)
{
    for(Mob* member : data.Map->party()->members())
    {
        member->changeStamina(member->maxStamina() - member->stamina());
    }
    data.Message = "You are fighting fit";
    return data;
}
PlayStateContainer& inspectHut(MapObject* context, PlayStateContainer& data)
{
    data.Message = Strings::HutDescription;
    data.State = PlayState::Message;
    return data;
}

/**
 * A place where you can safely rest.
 */
const TerrainTemplate GetHut()
{
    TerrainTemplate result;
    result.ImagePath = RESOURCE_LOCATION + "hut.png";
    result.IsDense = false;
    result.OnEnter = healAtHut;
    result.OnInspect = inspectHut;
    return result;
};
const TerrainTemplate Data::Hut = GetHut();
//}
