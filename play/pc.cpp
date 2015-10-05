#include "pc.h"

using namespace Play;


//{ Lifecycle
PC::PC(const Resources::PCTemplate& tmpl)
: Mob(tmpl, MobType::PlayerCharacter)
{
    _name = tmpl.Name;
    _memberCode = tmpl.MemberCode;
    otherCommands().push_back(new Command("Flee", Resources::Commands::FLEE));
    _jobClass = JobClass(tmpl.Class);
}

PC::~PC(void)
{
}
//}

//{ Properties
/**
 * Returns the maximum number of runes per spell a pc can have.
 */
natural PC::runeSlots(void) const { return _runeSlots; }

/**
 * The character's name.
 */
const std::string& PC::name(void) const { return _name; }

/**
 * Identifies which character this mob represents.
 */
Resources::PartyMemberCode PC::memberCode(void) const { return _memberCode; }

//}

//{ Methods
/**
 * Returns the maximum number of spells a pc can keep at one time.
 */
natural PC::spellSlots(void) const
{
    return _spellSlots > spells().size()
    ? spells().size() + 1
    : _spellSlots;
}

const std::string& PC::className(void) const
{
    return _jobClass.name();
}

const Persistence::SavedObjectCode PC::classCode(void) const
{
    return _jobClass.code();
}

bool PC::isPlayerControlled(void) const
{
    return true;
}

void PC::endCombat(void)
{
    applySpoils();
    Mob::endCombat();
}

/**
 * After an enemy is defeated add any experience points gained.
 */
void PC::getSpoils(int reward)
{
    _staminaEXP += floor(reward * _jobClass.staminaGrowthCoefficient());
    _skillEXP += floor(reward * _jobClass.skillGrowthCoefficient());
    _speedEXP += floor(reward * _jobClass.speedGrowthCoefficient());
    _defenceEXP += floor(reward * _jobClass.defenceGrowthCoefficient());
    _resistanceEXP += floor(reward * _jobClass.resistanceGrowthCoefficient());
}

/**
 * When combat finishes, increase any stats that have broken the threshold.
 */
void PC::applySpoils(void)
{
    maxStamina(maxStamina() + getStaminaGain());
    _staminaEXP = _staminaEXP % STAT_INCREASE_THRESHOLD;

    defaultSkill(defaultSkill() + getSkillGain() / 100.0);
    _skillEXP = _skillEXP % STAT_INCREASE_THRESHOLD;

    defaultSpeed(defaultSpeed() + getSpeedGain() / 100.0);
    _speedEXP = _speedEXP % STAT_INCREASE_THRESHOLD;

    defaultResistance(defaultResistance() + getResistanceGain() / 100.0);
    _resistanceEXP = _resistanceEXP % STAT_INCREASE_THRESHOLD;

    defaultDefence(defaultDefence() + getDefenceGain() / 100.0);
    _defenceEXP = _defenceEXP % STAT_INCREASE_THRESHOLD;
}

int PC::getStaminaGain(void) const
{
    return _staminaEXP / STAT_INCREASE_THRESHOLD;
}

int PC::getSpeedGain(void) const
{
    return _speedEXP / STAT_INCREASE_THRESHOLD;
}

int PC::getSkillGain(void) const
{
    return _skillEXP / STAT_INCREASE_THRESHOLD;
}

int PC::getResistanceGain(void) const
{
    return _resistanceEXP / STAT_INCREASE_THRESHOLD;
}

int PC::getDefenceGain(void) const
{
    return _defenceEXP / STAT_INCREASE_THRESHOLD;
}

//}



