#include "saveLoad.h"

using namespace Persistence;

SaveLoad::SaveLoad(std::string path_)
{
    _path = path_;
}

void SaveLoad::save(const PC& pc)
{
    std::vector<char> data = std::vector<char>();

    // Start with the PC location.
    data.push_back(SavedObjectCode::PCPosition);
    data.push_back(pc.x());
    data.push_back(pc.y());

    for (unsigned int i = 0; i < pc.spells()->size(); i++)
    {
        data.push_back(SavedObjectCode::NewSpell);

        for (Word* word : pc.spells()->at(i).components())
        {
            if (word == &Commands::FASTER)
                data.push_back(SavedObjectCode::FasterRune);
            else if (word == &Commands::FASTEST)
                data.push_back(SavedObjectCode::FastestRune);
            else if (word == &Commands::CASTER)
                data.push_back(SavedObjectCode::CasterRune);
            else if (word == &Commands::FRESHEST)
                data.push_back(SavedObjectCode::FreshestRune);
            else if (word == &Commands::HASTEN)
                data.push_back(SavedObjectCode::HastenRune);
            else if (word == &Commands::HEAL)
                data.push_back(SavedObjectCode::HealRune);
            else if (word == &Commands::HEAVIER)
                data.push_back(SavedObjectCode::HeavierRune);
            else if (word == &Commands::HURT)
                data.push_back(SavedObjectCode::HurtRune);
            else if (word == &Commands::LIGHTER)
                data.push_back(SavedObjectCode::LighterRune);
            else if (word == &Commands::SICKEST)
                data.push_back(SavedObjectCode::SickestRune);
            else if (word == &Commands::SLOW)
                data.push_back(SavedObjectCode::SlowRune);
            else if (word == &Commands::SLOWEST)
                data.push_back(SavedObjectCode::SlowestRune);
            else if (word == &Commands::ENEMY)
                data.push_back(SavedObjectCode::EnemyRune);

            else if (word == &Commands::DEFEND)
                data.push_back(SavedObjectCode::DefendRune);
            else if (word == &Commands::GUARDED)
                data.push_back(SavedObjectCode::GuardedRune);
            else if (word == &Commands::WARD)
                data.push_back(SavedObjectCode::WardedRune);
            else if (word == &Commands::WARDED)
                data.push_back(SavedObjectCode::WardedRune);
            else if (word == &Commands::ENDANGER)
                data.push_back(SavedObjectCode::EndangerRune);
            else if (word == &Commands::VULNERABLE)
                data.push_back(SavedObjectCode::VulnerableRune);
            else if (word == &Commands::EXPOSED)
                data.push_back(SavedObjectCode::ExposedRune);
            else if (word == &Commands::EXPOSE)
                data.push_back(SavedObjectCode::ExposeRune);
            else if (word == &Commands::ALL)
                data.push_back(SavedObjectCode::AllRune);
            else
                throw;
        }
    }

    Util::writeFile(_path.c_str(), data);
}

/**
 * Loads information about a pc from a save file.
 * @param pc The character that will have properties assigned from the save file.
 */
void SaveLoad::load(PC& pc) const
{
    std::vector<char> data = Util::readFile(_path.c_str());
    std::vector<Spell> spells = std::vector<Spell>();

    Spell workingSpell;
    bool spellInProgress = false;
    for (unsigned int i = 0; i < data.size(); i++)
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
                pc.x(int(data.at(i+1)));
                pc.y(int(data.at(i+2)));
                i+= 2;
                break;
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
            default: throw;
        }

    }

    if (spellInProgress)
    {
        workingSpell.resolve();
        spells.push_back(workingSpell);
    }

    for (Spell s : spells)
        pc.spells()->push_back(Command("", s));
}