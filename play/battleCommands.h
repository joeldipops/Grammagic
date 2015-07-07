#ifndef BATTLECOMMANDS_H_INCLUDED
#define BATTLECOMMANDS_H_INCLUDED

#include "mob.h"
#include "../combat/battleField.h"
#include "../mapObject.h"
#include "../combat/noun.h"
#include "../combat/verb.h"
#include "../combat/modifier.h"

class Commands
{
    public:
        static int FLEE(Command*, Mob*, BattleField* field)
        {
            field->isInCombat(false);
            return 0;
        };

        static Noun SELF;
        static Noun ENEMY_WEAKEST;
        static Verb WEAKEN;
        static Verb HEAL;
        static Noun ENEMY_STRONGEST;

    private:
        static MapObject* self (Mob* caster, BattleField*)
        {
            return (MapObject*) caster;
        };

        static MapObject* enemyWeakest(Mob* caster, BattleField* battleField)
        {
            Mob* result = nullptr;
            for(int i = 0; i < int(battleField->mobs().size()); i++)
            {
                Mob* mob = battleField->mobs().at(i);

                if (mob->type() != MobType::Hostile)
                    continue;

                if (result == nullptr)
                    result = mob;
                else if (mob->stamina() < result->stamina())
                    result = mob;

            }

            return result;
        };

        static MapObject* enemyStrongest(Mob* caster, BattleField* battleField)
        {
            Mob* result = nullptr;
            for(int i = 0; i < int(battleField->mobs().size()); i++)
            {
                Mob* mob = battleField->mobs().at(i);

                if (mob->type() != MobType::Hostile)
                    continue;

                if (result == nullptr)
                    result = mob;
                else if (mob->stamina() > result->stamina())
                    result = mob;

            }

            return result;
        };

        static void weaken(MapObject* source, MapObject* target, int cost, int effect)
        {
            source->changeStamina(cost*-1);
            target->changeStamina(effect*-1);
        };

        static void strengthen(MapObject* source, MapObject* target, int cost, int effect)
        {
            source->changeStamina(cost);
            target->changeStamina(effect);
        };
};

Noun Commands::SELF = Noun(self, std::string("SELF"), Modifier(.8), Modifier(1.0), Modifier(1.0));
Noun Commands::ENEMY_WEAKEST = Noun(enemyWeakest, std::string("ENEMY_WEAKEST"), Modifier(1.0), Modifier(1.0), Modifier(1.0));
Noun Commands::ENEMY_STRONGEST = Noun(enemyStrongest, std::string("ENEMY_STRONGEST"), Modifier(1.0), Modifier(1.0), Modifier(1.0));
Verb Commands::WEAKEN = Verb(weaken, std::string("WEAKEN"), Modifier(50), Modifier(10), Modifier(2000), -2, 1, 0);
Verb Commands::HEAL = Verb(strengthen, std::string("HEAL"), Modifier(50), Modifier(10, -1.0), Modifier(2000), -1, 1, 0);

#endif


