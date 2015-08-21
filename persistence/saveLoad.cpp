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
        data.push_back(SavedObjectCode::NewMember);
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
        result.push_back(SavedObjectCode::NewSpell);

        for (Word* word : pc.spells()->at(i).components())
        {
            if (word == &Commands::FASTER)
                result.push_back(SavedObjectCode::FasterRune);
            else if (word == &Commands::FASTEST)
                result.push_back(SavedObjectCode::FastestRune);
            else if (word == &Commands::CASTER)
                result.push_back(SavedObjectCode::CasterRune);
            else if (word == &Commands::FRESHEST)
                result.push_back(SavedObjectCode::FreshestRune);
            else if (word == &Commands::HASTEN)
                result.push_back(SavedObjectCode::HastenRune);
            else if (word == &Commands::HEAL)
                result.push_back(SavedObjectCode::HealRune);
            else if (word == &Commands::HEAVIER)
                result.push_back(SavedObjectCode::HeavierRune);
            else if (word == &Commands::HURT)
                result.push_back(SavedObjectCode::HurtRune);
            else if (word == &Commands::LIGHTER)
                result.push_back(SavedObjectCode::LighterRune);
            else if (word == &Commands::SICKEST)
                result.push_back(SavedObjectCode::SickestRune);
            else if (word == &Commands::SLOW)
                result.push_back(SavedObjectCode::SlowRune);
            else if (word == &Commands::SLOWEST)
                result.push_back(SavedObjectCode::SlowestRune);
            else if (word == &Commands::ENEMY)
                result.push_back(SavedObjectCode::EnemyRune);
            else if (word == &Commands::DEFEND)
                result.push_back(SavedObjectCode::DefendRune);
            else if (word == &Commands::GUARDED)
                result.push_back(SavedObjectCode::GuardedRune);
            else if (word == &Commands::WARD)
                result.push_back(SavedObjectCode::WardRune);
            else if (word == &Commands::WARDED)
                result.push_back(SavedObjectCode::WardedRune);
            else if (word == &Commands::ENDANGER)
                result.push_back(SavedObjectCode::EndangerRune);
            else if (word == &Commands::VULNERABLE)
                result.push_back(SavedObjectCode::VulnerableRune);
            else if (word == &Commands::EXPOSED)
                result.push_back(SavedObjectCode::ExposedRune);
            else if (word == &Commands::EXPOSE)
                result.push_back(SavedObjectCode::ExposeRune);
            else if (word == &Commands::ALL)
                result.push_back(SavedObjectCode::AllRune);
            else if (word == &Commands::ENHANCE)
                result.push_back(SavedObjectCode::EnhanceRune);
            else if (word == &Commands::IMPAIR)
                result.push_back(SavedObjectCode::ImpairRune);
            else if (word == &Commands::STRONGEST)
                result.push_back(SavedObjectCode::StrongestRune);
            else if (word == &Commands::WEAKEST)
                result.push_back(SavedObjectCode::WeakestRune);
            else if (word == &Commands::ALLY)
                result.push_back(SavedObjectCode::AllyRune);
            else if (word == &Commands::MEMBER)
                result.push_back(SavedObjectCode::MemberRune);
            else
                throw;
        }
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

            case SavedObjectCode::PCPosition:
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
            case SavedObjectCode::NewMember: {
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
            case SavedObjectCode::NewSpell:
                if (spellInProgress)
                {
                    workingSpell.resolve();
                    spells.push_back(workingSpell);
                }

                spellInProgress = true;
                workingSpell = Spell();
                break;
            case SavedObjectCode::CasterRune:
                if (!spellInProgress)
                    throw;
                workingSpell.addComponent(&Commands::CASTER);
                break;
            case SavedObjectCode::EnemyRune:
                if (!spellInProgress)
                    throw;
                workingSpell.addComponent(&Commands::ENEMY);
                break;
            case SavedObjectCode::FastestRune:
                if (!spellInProgress)
                    throw;
                workingSpell.addComponent(&Commands::FASTEST);
                break;
            case SavedObjectCode::FasterRune:
                if (!spellInProgress)
                    throw;
                workingSpell.addComponent(&Commands::FASTER);
                break;
            case SavedObjectCode::FreshestRune:
                if (!spellInProgress)
                    throw;
                workingSpell.addComponent(&Commands::FRESHEST);
                break;
            case SavedObjectCode::SlowRune:
                if (!spellInProgress)
                    throw;
                workingSpell.addComponent(&Commands::SLOW);
                break;
            case SavedObjectCode::SlowestRune:
                if (!spellInProgress)
                    throw;
                workingSpell.addComponent(&Commands::SLOWEST);
                break;
            case SavedObjectCode::SickestRune:
                if (!spellInProgress)
                    throw;
                workingSpell.addComponent(&Commands::SICKEST);
                break;
            case SavedObjectCode::LighterRune:
                if (!spellInProgress)
                    throw;
                workingSpell.addComponent(&Commands::LIGHTER);
                break;
            case SavedObjectCode::HurtRune:
                if (!spellInProgress)
                    throw;
                workingSpell.addComponent(&Commands::HURT);
                break;
            case SavedObjectCode::HeavierRune:
                if (!spellInProgress)
                    throw;
                workingSpell.addComponent(&Commands::HEAVIER);
                break;
            case SavedObjectCode::HealRune:
                if (!spellInProgress)
                    throw;
                workingSpell.addComponent(&Commands::HEAL);
                break;
            case SavedObjectCode::HastenRune:
                if (!spellInProgress)
                    throw;
                workingSpell.addComponent(&Commands::HASTEN);
                break;
            case SavedObjectCode::DefendRune:
                if (!spellInProgress)
                    throw;
                workingSpell.addComponent(&Commands::DEFEND);
                break;
            case SavedObjectCode::WardRune:
                if (!spellInProgress)
                    throw;
                workingSpell.addComponent(&Commands::WARD);
                break;
            case SavedObjectCode::ExposeRune:
                if (!spellInProgress)
                    throw;
                workingSpell.addComponent(&Commands::EXPOSED);
                break;
            case SavedObjectCode::EndangerRune:
                if (!spellInProgress)
                    throw;
                workingSpell.addComponent(&Commands::ENDANGER);
                break;
            case SavedObjectCode::GuardedRune:
                if (!spellInProgress)
                    throw;
                workingSpell.addComponent(&Commands::GUARDED);
                break;
            case SavedObjectCode::VulnerableRune:
                if (!spellInProgress)
                    throw;
                workingSpell.addComponent(&Commands::VULNERABLE);
                break;
            case SavedObjectCode::WardedRune:
                if (!spellInProgress)
                    throw;
                workingSpell.addComponent(&Commands::WARDED);
                break;
            case SavedObjectCode::ExposedRune:
                if (!spellInProgress)
                    throw;
                workingSpell.addComponent(&Commands::EXPOSED);
                break;
            case SavedObjectCode::AllRune:
                if (!spellInProgress)
                    throw;
                workingSpell.addComponent(&Commands::ALL);
                break;

            case SavedObjectCode::EnhanceRune:
                if (!spellInProgress)
                    throw;
                workingSpell.addComponent(&Commands::ENHANCE);
                break;
            case SavedObjectCode::ImpairRune:
                if (!spellInProgress)
                    throw;
                workingSpell.addComponent(&Commands::IMPAIR);
                break;
            case SavedObjectCode::StrongestRune:
                if (!spellInProgress)
                    throw;
                workingSpell.addComponent(&Commands::STRONGEST);
                break;
            case SavedObjectCode::WeakestRune:
                if (!spellInProgress)
                    throw;
                workingSpell.addComponent(&Commands::WEAKEST);
                break;
            case SavedObjectCode::AllyRune:
                if (!spellInProgress)
                    throw;
                workingSpell.addComponent(&Commands::ALLY);
                break;
            case SavedObjectCode::MemberRune:
                if (!spellInProgress)
                    throw;
                workingSpell.addComponent(&Commands::MEMBER);
                break;
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
