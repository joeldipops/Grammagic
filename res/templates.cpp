#include "templates.h"
#include "battleCommands.h"
#include "strings.h"
#include "../play/playStateManager.h"
using namespace Templates;

//{Runes

static Combatable* all(Mob* caster, BattleField* field, const std::vector<Combatable*>& candidates, SpellData&)
{
    TargetAll* result = new TargetAll(candidates);

    bool isPlayerAllied = field->areAllied(caster, field->pcs().at(0));

    // Should the target be considered an ally or an of the caster?
    natural allyCount = 0;
    for (const Combatable* c : candidates)
    {
        if (field->areAllied(caster, c))
            allyCount++;
    }


    if (allyCount > candidates.size() / 2.0)
        field->addToField(result, isPlayerAllied);
    else
        field->addToField(result, !isPlayerAllied);

    return result;
}
const RuneTemplate GetALL()
{
    RuneTemplate result = RuneTemplate();
    result.Name = "ALL";
    result.ImagePath = "";
    result.GetTarget ;
    result.GetTargetCandidates;
    result.SelectTargetFromCandidates = all;
    result.PerformAction;

    result.AddEffect = 0;
    result.AddCost = 0;
    result.AddDuration = 0;

    result.ModEffect = 1.0;
    result.ModCost = 1.0;
    result.ModDuration = 1.0;
    return result;
};
const RuneTemplate Templates::Data::ALL = GetALL();

static Combatable* self (Mob* caster, BattleField*, SpellData&)
{
    return (Combatable*) caster;
};
const RuneTemplate GetCASTER()
{
    RuneTemplate result = RuneTemplate();
    result.Name = "CASTER";
    result.ImagePath = "";
    result.GetTarget = self;
    result.GetTargetCandidates;
    result.SelectTargetFromCandidates;
    result.PerformAction;

    result.AddEffect = 0;
    result.AddCost = 0;
    result.AddDuration = 0;

    result.ModEffect = 1.0;
    result.ModCost = 1.0;
    result.ModDuration = 1.0;
    return result;
};
const RuneTemplate Templates::Data::CASTER = GetCASTER();


static std::vector<Combatable*> enemies(Mob* caster, BattleField* battleField, SpellData&)
{
    std::vector<Combatable*> result;
    std::vector<Mob*> candidates;

    if (caster->type() == MobType::PlayerCharacter)
        candidates =  battleField->hostiles();
    else
        candidates = battleField->pcs();

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
    result.ImagePath = "";
    result.GetTarget;
    result.GetTargetCandidates = enemies;
    result.SelectTargetFromCandidates;
    result.PerformAction;

    result.AddEffect = 0;
    result.AddCost = 0;
    result.AddDuration = 0;

    result.ModEffect = 1.0;
    result.ModCost = 1.0;
    result.ModDuration = 1.0;
    return result;
};
const RuneTemplate Templates::Data::ENEMY = GetENEMY();

static std::vector<Combatable*> allies(Mob* caster, BattleField* battleField, SpellData&)
{
    std::vector<Combatable*> result;
    std::vector<Mob*> candidates;

    if (caster->type() == MobType::PlayerCharacter)
        candidates = battleField->pcs();
    else
        candidates =  battleField->hostiles();

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
    result.ImagePath = "";
    result.GetTarget;
    result.GetTargetCandidates = allies;
    result.SelectTargetFromCandidates;
    result.PerformAction;

    result.AddEffect = 0;
    result.AddCost = 0;
    result.AddDuration = 0;

    result.ModEffect = 1.0;
    result.ModCost = 1.0;
    result.ModDuration = 1.0;
    return result;
};
const RuneTemplate Templates::Data::ALLY = GetALLY();

static std::vector<Combatable*> members(Mob* caster, BattleField* battleField, SpellData& data)
{
    std::vector<Combatable*> result;
    std::vector<Mob*> candidates;

    if (caster->type() == MobType::PlayerCharacter)
        candidates = battleField->pcs();
    else
        candidates =  battleField->hostiles();

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
    result.ImagePath = "";
    result.GetTarget;
    result.GetTargetCandidates = members;
    result.SelectTargetFromCandidates = all;
    result.PerformAction;

    result.AddEffect = 0;
    result.AddCost = 0;
    result.AddDuration = 0;

    result.ModEffect = 1.0;
    result.ModCost = 1.0;
    result.ModDuration = 1.0;
    return result;
};
const RuneTemplate Templates::Data::MEMBER = GetMEMBER();

SpellData& modifySpell(SpellData& data, float metaCost, float metaEffect, float metaDuration)
{
    data.cost *= 1 + metaCost;
    data.duration *= 1 + metaEffect;
    data.effect *= 1 + metaDuration;
    return  data;
}


const RuneTemplate GetSPELL()
{
    RuneTemplate result = RuneTemplate();
    result.Name = "SPELL";
    result.ImagePath = "";
    result.GetTarget ;
    result.GetTargetCandidates;
    result.SelectTargetFromCandidates;
    result.PerformAction;

    result.AddEffect = 0;
    result.AddCost = 0;
    result.AddDuration = 0;

    result.ModEffect = 1.0;
    result.ModCost = 1.0;
    result.ModDuration = 1.0;
    return result;
};
const RuneTemplate Templates::Data::SPELL = GetSPELL();

Combatable* most(Mob* caster, BattleField* battleField, const std::vector<Combatable*>& candidates, SpellData& data)
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
            if (data.isHigh)
            {
                if (mobValue < resultValue)
                    result = mob;
            }
            else
            {
                if (mobValue > resultValue)
                    result = mob;
            }
        }
    }

    return result;
};

void changeStat(Combatable* source, Combatable* target, int cost, int effect, SpellData& data)
{
    if (!data.isHigh)
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
    result.ImagePath = "";
    result.GetTarget ;
    result.GetTargetCandidates;
    result.SelectTargetFromCandidates;
    result.PerformAction;
    result.MetaAction = heavySpell;

    result.AddEffect = 0;
    result.AddCost = 0;
    result.AddDuration = 0;

    result.ModEffect = 1.0;
    result.ModCost = 1.0;
    result.ModDuration = 1.0;
    return result;
};
const RuneTemplate Templates::Data::HIGH = GetHIGH();

SpellData& lightSpell(SpellData& data)
{
    return modifySpell(data, -.5, -.5, 0);
}

const RuneTemplate GetLOW()
{
    RuneTemplate result = RuneTemplate();
    result.Name = "LOW";
    result.ImagePath = "";
    result.GetTarget ;
    result.GetTargetCandidates;
    result.SelectTargetFromCandidates = all;
    result.PerformAction;
    result.MetaAction = lightSpell;

    result.AddEffect = 0;
    result.AddCost = 0;
    result.AddDuration = 0;

    result.ModEffect = 1.0;
    result.ModCost = 1.0;
    result.ModDuration = 1.0;
    return result;
};
const RuneTemplate Templates::Data::LOW = GetLOW();


Combatable* mostStamina (Mob* caster, BattleField* battleField, const std::vector<Combatable*>& candidates, SpellData& data)
{
    data.stat = Stat::STAMINA;
    return most(caster, battleField, candidates, data);
}
const RuneTemplate GetSTAMINA()
{
    RuneTemplate result = RuneTemplate();
    result.Name = "STAMINA";
    result.ImagePath = "";
    result.GetTarget ;
    result.GetTargetCandidates;
    result.SelectTargetFromCandidates = mostStamina;
    result.PerformAction;

    result.AddEffect = 0;
    result.AddCost = 0;
    result.AddDuration = 0;

    result.ModEffect = 1.0;
    result.ModCost = 1.0;
    result.ModDuration = 1.0;
    return result;
};
const RuneTemplate Templates::Data::STAMINA = GetSTAMINA();

SpellData& fastSpell(SpellData& data)
{
    return modifySpell(data, 0, 0, -.25);
}

Combatable* mostSpeed (Mob* caster, BattleField* battleField, const std::vector<Combatable*>& candidates, SpellData& data)
{
    data.stat = Stat::SPEED;
    return most(caster, battleField, candidates, data);
}
const RuneTemplate GetSPEED()
{
    RuneTemplate result = RuneTemplate();
    result.Name = "SPEED";
    result.ImagePath = "";
    result.GetTarget ;
    result.GetTargetCandidates;
    result.SelectTargetFromCandidates = mostSpeed;
    result.PerformAction;
    result.MetaAction = fastSpell;

    result.AddEffect = 0;
    result.AddCost = 0;
    result.AddDuration = 0;

    result.ModEffect = 1.0;
    result.ModCost = 1.0;
    result.ModDuration = 1.0;
    return result;
};
const RuneTemplate Templates::Data::SPEED = GetSPEED();

Combatable* mostDefence(Mob* caster, BattleField* battleField, const std::vector<Combatable*>& candidates, SpellData& data)
{
    data.stat = Stat::DEFENSE;
    return most(caster, battleField, candidates, data);
}
const RuneTemplate GetDEFENCE()
{
    RuneTemplate result = RuneTemplate();
    result.Name = "DEFENCE";
    result.ImagePath = "";
    result.GetTarget ;
    result.GetTargetCandidates;
    result.SelectTargetFromCandidates = mostDefence;
    result.PerformAction;

    result.AddEffect = 0;
    result.AddCost = 0;
    result.AddDuration = 0;

    result.ModEffect = 1.0;
    result.ModCost = 1.0;
    result.ModDuration = 1.0;
    return result;
};
const RuneTemplate Templates::Data::DEFENCE = GetDEFENCE();

Combatable* mostResistance(Mob* caster, BattleField* battleField, const std::vector<Combatable*>& candidates, SpellData& data)
{
    data.stat = Stat::RESISTANCE;
    return most(caster, battleField, candidates, data);
}
const RuneTemplate GetRESISTANCE()
{
    RuneTemplate result = RuneTemplate();
    result.Name = "RESISTANCE";
    result.ImagePath = "";
    result.GetTarget ;
    result.GetTargetCandidates;
    result.SelectTargetFromCandidates = mostResistance;
    result.PerformAction;

    result.AddEffect = 0;
    result.AddCost = 0;
    result.AddDuration = 0;

    result.ModEffect = 1.0;
    result.ModCost = 1.0;
    result.ModDuration = 1.0;
    return result;
};
const RuneTemplate Templates::Data::RESISTANCE = GetRESISTANCE();

Combatable* mostSkill(Mob* caster, BattleField* battleField, const std::vector<Combatable*>& candidates, SpellData& data)
{
    data.stat = Stat::SKILL;
    return most(caster, battleField, candidates, data);
}
const RuneTemplate GetSKILL()
{
    RuneTemplate result = RuneTemplate();
    result.Name = "SKILL";
    result.ImagePath = "";
    result.GetTarget ;
    result.GetTargetCandidates;
    result.SelectTargetFromCandidates = all;
    result.PerformAction;

    result.AddEffect = 0;
    result.AddCost = 0;
    result.AddDuration = 0;

    result.ModEffect = 1.0;
    result.ModCost = 1.0;
    result.ModDuration = 1.0;
    return result;
};
const RuneTemplate Templates::Data::SKILL = GetSKILL();

//}


//{Party Members
PCTemplate GetA() // "Albert" archetype
{
    PCTemplate result;
    result.Name = Strings::AName;
    result.ImagePath = RESOURCE_LOCATION + "a-image.png";
    result.PortraitPath = RESOURCE_LOCATION + "a-portrait.png";
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

//{Types of Enemy
int aiAttack(Play::Mob* context, Play::BattleField* field)
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
