#include "saveLoad.h"
#include "../res/strings.h"
#include "../magic/spell.h"

using namespace Persistence;
using namespace Resources;

SaveLoad::SaveLoad(const std::string& path_)
{
    _path = path_;
}

void SaveLoad::save(const Party& party)
{
    std::vector<byte> data = std::vector<byte>();

    // Start with the PC location.
    data.push_back(SavedObjectCode::PC_POSITION);
    data.push_back(party.x());
    data.push_back(party.y());
    for (Rune* r : party.runeCollection())
        data.push_back(r->code());
    for (PC* pc : party.members())
    {
        data.push_back(SavedObjectCode::NEW_MEMBER);
        data.push_back(pc->memberCode());
        data.push_back(pc->classCode());
        pushNumeric(data, pc->maxStamina());
        pushNumeric(data, (unsigned short)(pc->defaultSkill() * 100));
        pushNumeric(data, (unsigned short)(pc->defaultSpeed() * 100));
        pushNumeric(data, (unsigned short)(pc->defaultResistance() * 100));
        pushNumeric(data, (unsigned short)(pc->defaultDefence() * 100));
        std::vector<byte> spells = getSpellBytes(*pc);
        data.insert(data.end(), spells.begin(), spells.end());
    }

    Util::writeFile(_path, data);
}

void SaveLoad::pushNumeric(std::vector<byte>& data, unsigned short value)
{
    std::vector<byte> bytes = Util::splitShort(value);
    data.insert(data.end(), bytes.begin(), bytes.end());
}

std::vector<byte> SaveLoad::getSpellBytes(const PC& pc) const
{
    std::vector<byte> result = std::vector<byte>();
    for (natural i = 0; i < pc.spells().size(); i++)
    {
        result.push_back(SavedObjectCode::NEW_SPELL);

        for (Rune* rune : pc.spells().at(i)->components())
            result.push_back(rune->code());
    }
    return result;
}

/**
 * Loads information about a pc from a save file.
 * @param pc The character that will have properties assigned from the save file.
 */
void SaveLoad::load(Party& party) const
{
    std::vector<byte> data = Util::readFile(_path.c_str());

    if (data.size() <= 0)
        return;

    std::vector<Spell*> spells = std::vector<Spell*>();

    Spell* workingSpell = nullptr;
    PC* pc = nullptr;
    bool spellInProgress = false;
    try {
    for (natural i = 0; i < data.size(); i++)
    {
        switch(SavedObjectCode(data.at(i)))
        {
            case SavedObjectCode::PC_POSITION:
                if (spellInProgress)
                {
                    workingSpell->resolve();
                    spells.push_back(workingSpell);
                }

                spellInProgress = false;
                party.x(int(data.at(++i)));
                party.y(int(data.at(++i)));
                break;
            case SavedObjectCode::NEW_MEMBER: {
                if (spellInProgress)
                {
                    workingSpell->resolve();
                    spells.push_back(workingSpell);

                    for (Spell* s : spells)
                        pc->addSpell(s);
                    spells = std::vector<Spell*>();
                }
                spellInProgress = false;
                Resources::PCTemplate t = Resources::PCTemplate();
                t.MemberCode = PartyMemberCode(data.at(++i));

                switch(t.MemberCode)
                {
                    case PartyMemberCode::A:
                        t.Name = Strings::AName;
                        t.ImagePath = RESOURCE_LOCATION + "a-image.png";
                        t.PortraitPath = RESOURCE_LOCATION + "a-portrait.png";
                        break;
                    case PartyMemberCode::B:
                        t.Name = Strings::BName;
                        t.ImagePath = RESOURCE_LOCATION + "b-image.png";
                        t.PortraitPath = RESOURCE_LOCATION + "b-portrait.png";
                        break;
                    case PartyMemberCode::C:
                        t.Name = Strings::CName;
                        t.ImagePath = RESOURCE_LOCATION + "c-image.png";
                        t.PortraitPath = RESOURCE_LOCATION + "c-portrait.png";
                        break;
                    default: throw;
                }

                SavedObjectCode classCode = SavedObjectCode(data.at(++i));
                switch(classCode)
                {
                    case SavedObjectCode::TANK_CLASS:
                        t.Class = Resources::Data::TANK; break;
                    case SavedObjectCode::WELLSPRING_CLASS:
                        t.Class = Resources::Data::WELLSPRING; break;
                    case SavedObjectCode::GLASSCANNON_CLASS:
                        t.Class = Resources::Data::GLASSCANON; break;
                    case SavedObjectCode::JACK_CLASS:
                        t.Class = Resources::Data::JACK; break;
                    case SavedObjectCode::GUARD_CLASS:
                        t.Class = Resources::Data::GUARD; break;
                    case SavedObjectCode::WASP_CLASS:
                        t.Class = Resources::Data::WASP; break;
                    default: throw;
                }

                t.Stamina = fuseShort(data.at(++i), data.at(++i));
                t.Skill = float(fuseShort(data.at(++i), data.at(++i)) / 100.0);
                t.Speed = float(fuseShort(data.at(++i), data.at(++i)) / 100.0);
                t.Resistance = float(fuseShort(data.at(++i), data.at(++i)) / 100.0);
                t.Defence = float(fuseShort(data.at(++i), data.at(++i)) / 100.0);
                pc = party.addMember(t);
                break;
            }
            case SavedObjectCode::NEW_SPELL:
                if (spellInProgress)
                {
                    workingSpell->resolve();
                    spells.push_back(workingSpell);
                }

                spellInProgress = true;
                workingSpell = new Spell();
                break;
            case SavedObjectCode::HIGH_RUNE:
                if (spellInProgress)
                {
                    for (Rune* r : party.runeCollection())
                    {
                        if (r->code() == HIGH_RUNE)
                        {
                            workingSpell->addComponent(r);
                            continue;
                        }
                    }
                }
                else
                    party.addRunes(new Rune(Resources::Data::HIGH));
                break;
            case SavedObjectCode::LOW_RUNE:
                if (spellInProgress)
                {
                    for (Rune* r : party.runeCollection())
                    {
                        if (r->code() == LOW_RUNE)
                        {
                            workingSpell->addComponent(r);
                            continue;
                        }
                    }
                }
                else
                    party.addRunes(new Rune(Resources::Data::LOW));
                break;
            case SavedObjectCode::CASTER_RUNE:
                if (spellInProgress)
                {
                    for (Rune* r : party.runeCollection())
                    {
                        if (r->code() == SavedObjectCode::CASTER_RUNE)
                        {
                            workingSpell->addComponent(r);
                            continue;
                        }
                    }
                }
                else
                    party.addRunes(new Rune(Resources::Data::CASTER));
                break;
            case SavedObjectCode::ALL_RUNE:
                if (spellInProgress)
                {
                    for (Rune* r : party.runeCollection())
                    {
                        if (r->code() == SavedObjectCode::ALL_RUNE)
                        {
                            workingSpell->addComponent(r);
                            continue;
                        }
                    }
                }
                else
                    party.addRunes(new Rune(Resources::Data::ALL));
                break;
            case SavedObjectCode::ANY_RUNE:
                if (spellInProgress)
                {
                    for (Rune* r : party.runeCollection())
                    {
                        if (r->code() == SavedObjectCode::ANY_RUNE)
                        {
                            workingSpell->addComponent(r);
                            continue;
                        }
                    }
                }
                else
                    party.addRunes(new Rune(Resources::Data::ANY));
                break;
            case SavedObjectCode::ALLY_RUNE:
                if (spellInProgress)
                {
                    for (Rune* r : party.runeCollection())
                    {
                        if (r->code() == SavedObjectCode::ALLY_RUNE)
                        {
                            workingSpell->addComponent(r);
                            continue;
                        }
                    }
                }
                else
                    party.addRunes(new Rune(Resources::Data::ALLY));
                break;
            case SavedObjectCode::MEMBER_RUNE:
                if (spellInProgress)
                {
                    for (Rune* r : party.runeCollection())
                    {
                        if (r->code() == SavedObjectCode::MEMBER_RUNE)
                        {
                            workingSpell->addComponent(r);
                            continue;
                        }
                    }
                }
                else
                    party.addRunes(new Rune(Resources::Data::MEMBER));
                break;
            case SavedObjectCode::ENEMY_RUNE:
                if (spellInProgress)
                {
                    for (Rune* r : party.runeCollection())
                    {
                        if (r->code() == SavedObjectCode::ENEMY_RUNE)
                        {
                            workingSpell->addComponent(r);
                            continue;
                        }
                    }
                }
                else
                    party.addRunes(new Rune(Resources::Data::ENEMY));
                break;
            case SavedObjectCode::STAMINA_RUNE:
                if (spellInProgress)
                {
                    for (Rune* r : party.runeCollection())
                    {
                        if (r->code() == SavedObjectCode::STAMINA_RUNE)
                        {
                            workingSpell->addComponent(r);
                            continue;
                        }
                    }
                }
                else
                    party.addRunes(new Rune(Resources::Data::STAMINA));
                break;
            case SavedObjectCode::SKILL_RUNE:
                if (spellInProgress)
                {
                    for (Rune* r : party.runeCollection())
                    {
                        if (r->code() == SavedObjectCode::SKILL_RUNE)
                        {
                            workingSpell->addComponent(r);
                            continue;
                        }
                    }
                }
                else
                    party.addRunes(new Rune(Resources::Data::SKILL));
                break;
            case SavedObjectCode::SPEED_RUNE:
                if (spellInProgress)
                {
                    for (Rune* r : party.runeCollection())
                    {
                        if (r->code() == SavedObjectCode::SPEED_RUNE)
                        {
                            workingSpell->addComponent(r);
                            continue;
                        }
                    }
                }
                else
                    party.addRunes(new Rune(Resources::Data::SPEED));
                break;
            case SavedObjectCode::RESISTANCE_RUNE:
                if (spellInProgress)
                {
                    for (Rune* r : party.runeCollection())
                    {
                        if (r->code() == SavedObjectCode::RESISTANCE_RUNE)
                        {
                            workingSpell->addComponent(r);
                            continue;
                        }
                    }
                }
                else
                    party.addRunes(new Rune(Resources::Data::RESISTANCE));
                break;
            case SavedObjectCode::DEFENCE_RUNE:
                if (spellInProgress)
                {
                    for (Rune* r : party.runeCollection())
                    {
                        if (r->code() == SavedObjectCode::DEFENCE_RUNE)
                        {
                            workingSpell->addComponent(r);
                            continue;
                        }

                    }
                }
                else
                    party.addRunes(new Rune(Resources::Data::DEFENCE));
                break;
            default: throw;
        }
    }
    } catch (...) {
        if (workingSpell != nullptr)
            delete workingSpell;
        if (pc != nullptr)
            delete pc;
    }
    if (pc == nullptr)
        throw;
    if (spellInProgress)
    {
        workingSpell->resolve();
        spells.push_back(workingSpell);
    }

    for (Spell* s : spells)
        pc->addSpell(s);
}
