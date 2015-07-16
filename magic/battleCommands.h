#ifndef BATTLECOMMANDS_H_INCLUDED
#define BATTLECOMMANDS_H_INCLUDED

#include "battleField.h"
#include "../mapObject.h"
#include "properNoun.h"
#include "verb.h"
#include "modifier.h"
#include "adjective.h"
#include "noun.h"
#include "../play/mob.h"

namespace Magic
{
    class Commands
    {
        public:
            static int FLEE(Command*, Mob*, BattleField* field)
            {
                field->isInCombat(false);
                return 0;
            };

            static std::vector<Word*> allCommands;
            static ProperNoun SELF;
            static Noun ENEMY;
            static Verb WEAKEN;
            static Verb HEAL;
            static Verb HASTEN;
            static Verb SLOW;
            static Adjective FRESHEST;
            static Adjective SICKEST;
            static Adverb LIGHTER;
            static Adverb HEAVIER;
            static Adverb FASTER;

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
                if (caster->type() == MobType::PlayerCharacter)
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

            static void weaken(MapObject* source, MapObject* target, double cost, double effect)
            {
                source->changeStamina(ceil(cost*-1));
                target->changeStamina(ceil(effect*-1));
            };
            static void strengthen(MapObject* source, MapObject* target, double cost, double effect)
            {
                source->changeStamina(ceil(cost*-1));
                target->changeStamina(ceil(effect));
            };

            static void hasten(MapObject* source, MapObject* target, double cost, double effect)
            {
                source->changeStamina(ceil(cost * -1));
                target->changeSpeed(1 + (effect / 10));
            };

            static void slow(MapObject* source, MapObject* target, double cost, double effect)
            {
                source->changeStamina(ceil(cost * -1));
                target->changeSpeed(1 - (effect/ 10));
            };
    };
}
#endif
