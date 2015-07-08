#ifndef BATTLECOMMANDS_H_INCLUDED
#define BATTLECOMMANDS_H_INCLUDED

#include "mob.h"
#include "../combat/battleField.h"
#include "../mapObject.h"
#include "../combat/properNoun.h"
#include "../combat/verb.h"
#include "../combat/modifier.h"
#include "../combat/adjective.h"
#include "../combat/noun.h"

class Commands
{
    public:
        static int FLEE(Command*, Mob*, BattleField* field)
        {
            field->isInCombat(false);
            return 0;
        };

        static ProperNoun SELF;
        static Noun ENEMY;
        static Verb WEAKEN;
        static Verb HEAL;
        static Adjective STRONGEST;
        static Adjective WEAKEST;

    private:
        // Specific Nouns

        static MapObject* self (Mob* caster, BattleField*)
        {
            return (MapObject*) caster;
        };

        // General Nouns

        static std::vector<MapObject*> enemies(Mob* caster, BattleField* battleField)
        {
            std::vector<MapObject*> result;
            std::vector<Mob*> candidates;
            if (caster->type() == MobType::PC)
                candidates = battleField->hostiles();
            else
                candidates = battleField->pcs();

            for (Mob* m : candidates)
            {
                result.push_back((MapObject*) m);
            }

            return result;
        }


        // Adjectives

        static MapObject* weakest(Mob* caster, BattleField* battleField, std::vector<MapObject*> candidates)
        {
            Mob* result = nullptr;
            for(int i = 0; i < int(candidates.size()); i++)
            {
                Mob* mob = (Mob*) candidates.at(i);

                if (result == nullptr)
                    result = mob;
                else if (mob->stamina() < result->stamina())
                    result = mob;
            }

            return result;
        };

        static MapObject* strongest(Mob* caster, BattleField* battleField, std::vector<MapObject*> candidates)
        {
            Mob* result = nullptr;
            for(int i = 0; i < int(candidates.size()); i++)
            {
                Mob* mob = (Mob*) candidates.at(i);

                if (result == nullptr)
                    result = mob;
                else if (mob->stamina() > result->stamina())
                    result = mob;
            }

            return result;
        };

        // Verbs

        static void weaken(MapObject* source, MapObject* target, int cost, int effect)
        {
            source->changeStamina(cost*-1);
            target->changeStamina(effect*-1);
        };

        static void strengthen(MapObject* source, MapObject* target, int cost, int effect)
        {
            source->changeStamina(cost*-1);
            target->changeStamina(effect);
        };
};

Noun Commands::ENEMY = Noun(enemies, std::string("ENEMY"), Modifier(1.0), Modifier(1.0), Modifier(1.0));
Adjective Commands::WEAKEST = Adjective(weakest, std::string("WEAKEST"), Modifier(1.0), Modifier(1.0), Modifier(1.0));
Adjective Commands::STRONGEST = Adjective(strongest, std::string("WEAKEST"), Modifier(1.0), Modifier(1.0), Modifier(1.0));

ProperNoun Commands::SELF = ProperNoun(self, std::string("SELF"), Modifier(.8), Modifier(1.0), Modifier(1.0));
Verb Commands::WEAKEN = Verb(weaken, std::string("WEAKEN"), Modifier(50), Modifier(10), Modifier(2000), -2, 1, 0);
Verb Commands::HEAL = Verb(strengthen, std::string("HEAL"), Modifier(50), Modifier(10, 1.0), Modifier(2000), -1, 1, 0);

#endif


