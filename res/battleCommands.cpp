#include "battleCommands.h"
#include "../play/enemy.h"

using namespace Templates;
// Specific Nouns
//{
static Combatable* self (Mob* caster, BattleField*)
{
    return (Combatable*) caster;
};
//}


// General Nouns
//{
static std::vector<Combatable*> enemies(Mob* caster, BattleField* battleField)
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

static std::vector<Combatable*> allies(Mob* caster, BattleField* battleField)
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

static std::vector<Combatable*> members(Mob* caster, BattleField* battleField)
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
//}
// Adjectives
//{
static Combatable* all(Mob* caster, BattleField* field, std::vector<Combatable*> candidates)
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

static Combatable* exposed(Mob* caster, BattleField* battleField, std::vector<Combatable*> candidates)
{
    Combatable* result = nullptr;
    for(natural i = 0; i < candidates.size(); i++)
    {
        Combatable* mob = candidates.at(i);

        if (result == nullptr)
            result = mob;
        else if (mob->resistance() < result->resistance())
            result = mob;
    }

    return result;
};

static Combatable* warded(Mob* caster, BattleField* battleField, std::vector<Combatable*> candidates)
{
    Combatable* result = nullptr;
    for(natural i = 0; i < candidates.size(); i++)
    {
        Combatable* mob = (Combatable*) candidates.at(i);

        if (result == nullptr)
            result = mob;
        else if (mob->resistance() > result->resistance())
            result = mob;
    }

    return result;
};

static Combatable* vulnerable(Mob* caster, BattleField* battleField, std::vector<Combatable*> candidates)
{
    Combatable* result = nullptr;
    for(natural i = 0; i < candidates.size(); i++)
    {
        Combatable* mob = candidates.at(i);

        if (result == nullptr)
            result = mob;
        else if (mob->defence() < result->defence())
            result = mob;
    }

    return result;
};

static Combatable* guarded(Mob* caster, BattleField* battleField, std::vector<Combatable*> candidates)
{
    Combatable* result = nullptr;
    for(natural i = 0; i < candidates.size(); i++)
    {
        Combatable* mob = candidates.at(i);

        if (result == nullptr)
            result = mob;
        else if (mob->defence() > result->defence())
            result = mob;
    }

    return result;
};


static Combatable* freshest(Mob* caster, BattleField* battleField, std::vector<Combatable*> candidates)
{
    Combatable* result = nullptr;
    for(natural i = 0; i < candidates.size(); i++)
    {
        Combatable* mob = candidates.at(i);

        if (result == nullptr)
            result = mob;
        else if (mob->stamina() > result->stamina())
            result = mob;
    }

    return result;
};

static Combatable* sickest(Mob* caster, BattleField* battleField, std::vector<Combatable*> candidates)
{
    Combatable* result = nullptr;
    for(natural i = 0; i < candidates.size(); i++)
    {
        Combatable* mob = candidates.at(i);

        if (result == nullptr)
            result = mob;
        else if (mob->stamina() < result->stamina())
            result = mob;
    }

    return result;
};


static Combatable* strongest(Mob* caster, BattleField* battleField, std::vector<Combatable*> candidates)
{
    Combatable* result = nullptr;
    for(natural i = 0; i < candidates.size(); i++)
    {
        Combatable* mob = candidates.at(i);

        if (result == nullptr)
            result = mob;
        else if (mob->skill() < result->skill())
            result = mob;
    }

    return result;
};

static Combatable* weakest(Mob* caster, BattleField* battleField, std::vector<Combatable*> candidates)
{
    Combatable* result = nullptr;
    for(natural i = 0; i < candidates.size(); i++)
    {
        Combatable* mob = candidates.at(i);

        if (result == nullptr)
            result = mob;
        else if (mob->skill() > result->skill())
            result = mob;
    }

    return result;
};


static Combatable* fastest(Mob* caster, BattleField* battleField, std::vector<Combatable*> candidates)
{
    Combatable* result = nullptr;
    for(natural i = 0; i < candidates.size(); i++)
    {
        Combatable* mob = candidates.at(i);

        if (result == nullptr)
            result = mob;
        else if (mob->speed() > result->speed())
            result = mob;
    }

    return result;
};

static Combatable* slowest(Mob* caster, BattleField* battleField, std::vector<Combatable*> candidates)
{
    Combatable* result = nullptr;
    for(natural i = 0; i < candidates.size(); i++)
    {
        Combatable* mob = candidates.at(i);

        if (result == nullptr)
            result = mob;
        else if (mob->speed() < result->speed())
            result = mob;
    }

    return result;
};
//}

// Verbs
//{
static void defend(Combatable* source, Combatable* target, int cost, int effect)
{
    source->changeStamina(cost * -1);
    target->changeDefence(1 + (float(effect) / 100.0));
};

static void endanger(Combatable* source, Combatable* target, int cost, int effect)
{
    source->changeStamina(cost * -1);
    target->changeDefence(1 - (float(effect) / 100.0));
};

static void ward(Combatable* source, Combatable* target, int cost, int effect)
{
    source->changeStamina(cost * -1);
    target->changeResistance(1 + (float(effect) / 100.0));
};

static void expose(Combatable* source, Combatable* target, int cost, int effect)
{
    source->changeStamina(cost * -1);
    target->changeResistance(1 - (float(effect) / 100.0));
};

static void weaken(Combatable* source, Combatable* target, int cost, int effect)
{
    source->changeStamina(ceil(cost*-1));
    target->changeStamina(ceil(effect*-1));
};

static void strengthen(Combatable* source, Combatable* target, int cost, int effect)
{
    source->changeStamina(ceil(cost*-1));
    target->changeStamina(ceil(effect));
};

static void hasten(Combatable* source, Combatable* target, int cost, int effect)
{
    source->changeStamina(cost * -1);
    target->changeSpeed(1 + (float(effect) / 100.0));
};

static void slow(Combatable* source, Combatable* target, int cost, int effect)
{
    source->changeStamina(cost * -1);
    target->changeSpeed(1 - (float(effect) / 100.0));
};

static void enhance(Combatable* source, Combatable* target, int cost, int effect)
{
    source->changeStamina(cost * -1);
    target->changeSkill(1  + (float(effect) / 100.0));
};

static void impair(Combatable* source, Combatable* target, int cost, int effect)
{
    source->changeStamina(cost * -1);
    target->changeSkill(1 - (float(effect) / 100.0));
};
//}

// Non Spells
//{
int Commands::FLEE(Command*, Mob*, BattleField* field)
{
    field->isInCombat(false);
    return 0;
};

int Commands::ATTACK(Command* command, Mob* actor, BattleField* field)
{
    // PCs are squishy and can not attack physically
    if (actor->isPlayerControlled())
        return 0;

    Enemy* nmeActor = (Enemy*)actor;

    const int ATTACK_DURATION = 1000;
    // A limit to stop us looping forever if we are implausibly unlucky.
    const int randFail = 10;
    natural iter = 0;
    while(iter < randFail)
    {
        int i = rand() % field->combatants().size();
        Combatable* target = field->combatants().at(i);

        // Do damage to the target and then ollie outie.
        if (!field->areAllied(actor, target))
        {
            target->changeStamina(-1 * nmeActor->physicalStrength() * target->defence());
            return ATTACK_DURATION;
        }

        iter++;
    }
    return 0;
}
//}

std::vector<Word*> Commands::allCommands = std::vector<Word*>();


// A mob hostile to the caster.
Noun Commands::ENEMY = Noun(enemies, std::string("ENEMY"), Modifier(1.0), Modifier(1.0), Modifier(1.0));

// The caster
ProperNoun Commands::CASTER = ProperNoun(self, std::string("CASTER"), Modifier(.8), Modifier(1.0), Modifier(1.0));

// The caster's allies
Noun Commands::ALLY = Noun(allies, std::string("ALLY"), Modifier(1.0), Modifier(1.0), Modifier(1.0));

// Any member of the caster's party.
Noun Commands::MEMBER = Noun(members, std::string("MEMBER"), Modifier(1.0), Modifier(1.0), Modifier(1.0));

// The mob with the most or least stamina
Adjective Commands::FRESHEST = Adjective(freshest, std::string("FRESHEST"), Modifier(1.0), Modifier(1.0), Modifier(1.0));
Adjective Commands::SICKEST = Adjective(sickest, std::string("SICKEST"), Modifier(1.0), Modifier(1.0), Modifier(1.0));

// The fastest or slowest mob
Adjective Commands::FASTEST = Adjective(fastest, std::string("FASTEST"), Modifier(1.0), Modifier(1.0), Modifier(1.0));
Adjective Commands::SLOWEST = Adjective(slowest, std::string("SLOWEST"), Modifier(1.0), Modifier(1.0), Modifier(1.0));

// The mob with the most or least magical resistance
Adjective Commands::WARDED = Adjective(warded, std::string("WARDED"), Modifier(1.0), Modifier(1.0), Modifier(1.0));
Adjective Commands::EXPOSED = Adjective(exposed, std::string("EXPOSED"), Modifier(1.0), Modifier(1.0), Modifier(1.0));

// The mob with the most or least physical defence
Adjective Commands::GUARDED = Adjective(guarded, std::string("GUARDED"), Modifier(1.0), Modifier(1.0), Modifier(1.0));
Adjective Commands::VULNERABLE = Adjective(vulnerable, std::string("VULNERABLE"), Modifier(1.0), Modifier(1.0), Modifier(1.0));

// The mob with the most or least magical skill
Adjective Commands::STRONGEST = Adjective(strongest, std::string("STRONGEST"), Modifier(1.0), Modifier(1.0), Modifier(1.0));
Adjective Commands::WEAKEST = Adjective(weakest, std::string("WEAKEST"), Modifier(1.0), Modifier(1.0), Modifier(1.0));

// Equally distribute effect over all candidates
Adjective Commands::ALL = Adjective(all, std::string("ALL"), Modifier(1.1), Modifier(1.2), Modifier(1.05));

// Buff an ally's defence or reduce an enemy's
Verb Commands::DEFEND = Verb(defend, std::string("DEFEND"), Modifier(50), Modifier(10), Modifier(5000), -1, 1, 0, true);
Verb Commands::ENDANGER = Verb(endanger, std::string("ENDANGER"), Modifier(50), Modifier(10, 1.0), Modifier(5000), -1, 1, 0);

// Buff an ally's magical resistance or reduce an enemy's
Verb Commands::WARD = Verb(ward, std::string("WARD"), Modifier(50), Modifier(10), Modifier(5000), -1, 1, 0, true);
Verb Commands::EXPOSE = Verb(expose, std::string("EXPOSE"), Modifier(50), Modifier(10, 1.0), Modifier(5000), -1, 1, 0);

// Buff an ally's magical skill or reduce an enemy's
Verb Commands::ENHANCE= Verb(enhance, std::string("ENHANCE"), Modifier(50), Modifier(10), Modifier(5000), -1, 1, 0, true);
Verb Commands::IMPAIR = Verb(impair, std::string("IMPAIR"), Modifier(50), Modifier(10, 1.0), Modifier(5000), -1, 1, 0);

// Transfers health from one to the other.
Verb Commands::HURT = Verb(weaken, std::string("HURT"), Modifier(50), Modifier(10), Modifier(5000), -2, 1, 0);
Verb Commands::HEAL = Verb(strengthen, std::string("HEAL"), Modifier(50), Modifier(10, 1.0), Modifier(5000), -1, 1, 0, true);

// Trades health for speed.
Verb Commands::HASTEN = Verb(hasten, std::string("HASTEN"), Modifier(50), Modifier(10, 1.0), Modifier(10000), -2, 1, 1, true);
Verb Commands::SLOW = Verb(slow, std::string("SLOW"), Modifier(50), Modifier(10, 1.0), Modifier(10000), -1, 1, 1);

// Takes more to deal more, takes less to deal less
Adverb Commands::HEAVIER = Adverb(std::string("HEAVIER"), Modifier(1.25), Modifier(1.25), Modifier(1.0));
Adverb Commands::LIGHTER = Adverb(std::string("LIGHTER"), Modifier(.75), Modifier(.75), Modifier(1.0));

// Halves the duration, at a greater cost and some loss of effectiveness
Adverb Commands::FASTER = Adverb(std::string("FASTER"), Modifier(0.9), Modifier(1.5), Modifier(0.5));
