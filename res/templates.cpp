#include "templates.h"
using namespace Templates;

    PCTemplate GetA() // "Albert" archetype
    {
        PCTemplate result;
        result.Name = "Albert";
        result.ImagePath = RESOURCE_LOCATION + "pc.png";
        result.PortraitPath = RESOURCE_LOCATION + "magician.png";
        result.Stamina = 100;
        result.Speed = 2.0;
        result.Resistance = 1.1;
        result.Defence = 0.9;
        result.Skill = 1.5;
        return result;
    };
    const PCTemplate Data::A = GetA();

    PCTemplate GetB() // "Brienne" Archetype
    {
        PCTemplate result;
        result.Name = "Brienne";
        result.ImagePath = RESOURCE_LOCATION + "pc.png";
        result.PortraitPath = RESOURCE_LOCATION + "magician.png";
        result.Stamina = 200;
        result.Speed = 0.5;
        result.Resistance = 2.0;
        result.Defence = 2.0;
        result.Skill = 0.6;
        return result;
    };
    const PCTemplate Data::B = GetB();

    const PCTemplate GetC() //"All Rounder" archetype
    {
        PCTemplate result;
        result.Name = "Toph";
        result.ImagePath = RESOURCE_LOCATION + "pc.png";
        result.PortraitPath = RESOURCE_LOCATION + "magician.png";
        result.Stamina = 100;
        result.Speed = 1.0;
        result.Resistance = 1.0;
        result.Defence = 1.0;
        result.Skill = 1.0;
        return result;
    };
    const PCTemplate Data::C = GetC();
