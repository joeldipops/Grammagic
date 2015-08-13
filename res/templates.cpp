#include "templates.h"
using namespace Templates;

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


const EnemyTemplate GetE1()
{
    EnemyTemplate result;
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
    result.ImagePath = RESOURCE_LOCATION + "e2.png";
    result.Stamina = 100;
    result.Attack = 8;
    result.Speed = .8;
    result.Resistance = .9;
    result.Defence = 1.0;
    result.Skill = 1.2;
    result.RangeOfSense = 2;
    result.RangeOfSight = 2;
    result.AttackDelay = 3000;
    result.MovementDelay = 3000;
    return result;
};
const EnemyTemplate Data::E2 = GetE2();

const EnemyTemplate GetB1()
{
    EnemyTemplate result;
    result.ImagePath = RESOURCE_LOCATION + "b1.png";
    result.Stamina = 150;
    result.Attack = 25;
    result.Speed = 1.1;
    result.Resistance = 1.5;
    result.Defence = 2.0;
    result.Skill = 1.5;
    result.RangeOfSense = 0;
    result.RangeOfSight = 0;
    result.AttackDelay = 3000;
    result.MovementDelay = 3000;
    return result;
};
const EnemyTemplate Data::B1 = GetB1();

