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
