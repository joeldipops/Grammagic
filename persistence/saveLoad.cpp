#include "saveLoad.h"

using namespace Persistence;
using namespace Templates;

SaveLoad::SaveLoad(std::string path_)
{
    _path = path_;
}

void SaveLoad::save(const Party& party)
{
    std::vector<byte> data = std::vector<byte>();

   // Start with the PC location.
    //data.push_back(SavedObjectCode::PCPosition);
    //data.push_back(party.x());
    //data.push_back(party.y());
    for (PC* pc : party.members())
    {
        data.push_back(SavedObjectCode::NEW_MEMBER);
        data.push_back(pc->memberCode());
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
    for (natural i = 0; i < pc.spells()->size(); i++)
    {
        result.push_back(SavedObjectCode::NEW_SPELL);

        for (Rune* rune : pc.spells()->at(i).components())
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

    std::vector<Spell> spells = std::vector<Spell>();

    Spell workingSpell;
    PC* pc;
    bool spellInProgress = false;
    for (natural i = 0; i < data.size(); i++)
    {
        switch(SavedObjectCode(data.at(i)))
        {

            case SavedObjectCode::PC_POSITION:
                if (spellInProgress)
                {
                    workingSpell.resolve();
                    spells.push_back(workingSpell);
                }

                spellInProgress = false;
                //party.x(int(data.at(i+1)));
                //party.y(int(data.at(i+2)));
                i+= 2;
                break;
            case SavedObjectCode::NEW_MEMBER: {
                if (spellInProgress)
                {
                    workingSpell.resolve();
                    spells.push_back(workingSpell);

                    for (Spell s : spells)
                        pc->spells()->push_back(Command("", s));
                    spells = std::vector<Spell>();
                }
                spellInProgress = false;
                Templates::PCTemplate t = Templates::PCTemplate();
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
                    workingSpell.resolve();
                    spells.push_back(workingSpell);
                }

                spellInProgress = true;
                workingSpell = Spell();
                break;
            /*
            case SavedObjectCode::CASTER_RUNE:
                if (!spellInProgress)
                    throw;
                workingSpell.addComponent(&Commands::CASTER);
                break;
            */
            default: throw;
        }

    }
    if (pc == nullptr)
        throw;
    if (spellInProgress)
    {
        workingSpell.resolve();
        spells.push_back(workingSpell);
    }

    for (Spell s : spells)
        pc->spells()->push_back(Command("", s));
}
