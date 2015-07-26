#ifndef BATTLECOMMANDS_H_INCLUDED
#define BATTLECOMMANDS_H_INCLUDED

#include "../play/battleField.h"
#include "../mapObject.h"
#include "properNoun.h"
#include "verb.h"
#include "modifier.h"
#include "adjective.h"
#include "noun.h"
#include "../play/mob.h"
#include "targetAll.h"

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
            static ProperNoun CASTER;
            static Noun ENEMY;
            static Verb HURT;
            static Verb HEAL;
            static Verb HASTEN;
            static Verb SLOW;
            static Verb DEFEND; // Defence
            static Verb ENDANGER;
            static Verb WARD; // Resistance
            static Verb EXPOSE;
            static Verb ENHANCE; // Magic Skill
            static Verb IMPAIR;
            static Adjective GUARDED;
            static Adjective VULNERABLE;
            static Adjective WARDED;
            static Adjective EXPOSED;
            static Adjective FRESHEST;
            static Adjective SICKEST;
            static Adjective STRONGEST; // Magic Skill
            static Adjective WEAKEST;
            static Adverb LIGHTER;
            static Adverb HEAVIER;
            static Adverb FASTER;
            static Adjective FASTEST;
            static Adjective SLOWEST;
            static Adjective ALL;

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
            static MapObject* all(Mob* caster, BattleField* field, std::vector<MapObject*> candidates)
            {
                TargetAll* result = new TargetAll(candidates);
                field->toBin(result);
                return result;
            }

            static MapObject* exposed(Mob* caster, BattleField* battleField, std::vector<MapObject*> candidates)
            {
                MapObject* result = nullptr;
                for(unsigned int i = 0; i < candidates.size(); i++)
                {
                    MapObject* mob = candidates.at(i);

                    if (result == nullptr)
                        result = mob;
                    else if (mob->resistance() < result->resistance())
                        result = mob;
                }

                return result;
            };

            static MapObject* warded(Mob* caster, BattleField* battleField, std::vector<MapObject*> candidates)
            {
                MapObject* result = nullptr;
                for(unsigned int i = 0; i < candidates.size(); i++)
                {
                    MapObject* mob = (MapObject*) candidates.at(i);

                    if (result == nullptr)
                        result = mob;
                    else if (mob->resistance() > result->resistance())
                        result = mob;
                }

                return result;
            };

            static MapObject* vulnerable(Mob* caster, BattleField* battleField, std::vector<MapObject*> candidates)
            {
                MapObject* result = nullptr;
                for(unsigned int i = 0; i < candidates.size(); i++)
                {
                    MapObject* mob = candidates.at(i);

                    if (result == nullptr)
                        result = mob;
                    else if (mob->defence() < result->defence())
                        result = mob;
                }

                return result;
            };

            static MapObject* guarded(Mob* caster, BattleField* battleField, std::vector<MapObject*> candidates)
            {
                MapObject* result = nullptr;
                for(unsigned int i = 0; i < candidates.size(); i++)
                {
                    MapObject* mob = candidates.at(i);

                    if (result == nullptr)
                        result = mob;
                    else if (mob->defence() > result->defence())
                        result = mob;
                }

                return result;
            };


            static MapObject* freshest(Mob* caster, BattleField* battleField, std::vector<MapObject*> candidates)
            {
                MapObject* result = nullptr;
                for(unsigned int i = 0; i < candidates.size(); i++)
                {
                    MapObject* mob = candidates.at(i);

                    if (result == nullptr)
                        result = mob;
                    else if (mob->stamina() < result->stamina())
                        result = mob;
                }

                return result;
            };

            static MapObject* sickest(Mob* caster, BattleField* battleField, std::vector<MapObject*> candidates)
            {
                MapObject* result = nullptr;
                for(unsigned int i = 0; i < candidates.size(); i++)
                {
                    MapObject* mob = candidates.at(i);

                    if (result == nullptr)
                        result = mob;
                    else if (mob->stamina() > result->stamina())
                        result = mob;
                }

                return result;
            };


            static MapObject* strongest(Mob* caster, BattleField* battleField, std::vector<MapObject*> candidates)
            {
                MapObject* result = nullptr;
                for(unsigned int i = 0; i < candidates.size(); i++)
                {
                    MapObject* mob = candidates.at(i);

                    if (result == nullptr)
                        result = mob;
                    else if (mob->skill() < result->skill())
                        result = mob;
                }

                return result;
            };

            static MapObject* weakest(Mob* caster, BattleField* battleField, std::vector<MapObject*> candidates)
            {
                MapObject* result = nullptr;
                for(unsigned int i = 0; i < candidates.size(); i++)
                {
                    MapObject* mob = candidates.at(i);

                    if (result == nullptr)
                        result = mob;
                    else if (mob->skill() > result->skill())
                        result = mob;
                }

                return result;
            };


            static MapObject* fastest(Mob* caster, BattleField* battleField, std::vector<MapObject*> candidates)
            {
                MapObject* result = nullptr;
                for(unsigned int i = 0; i < candidates.size(); i++)
                {
                    MapObject* mob = candidates.at(i);

                    if (result == nullptr)
                        result = mob;
                    else if (mob->speed() > result->speed())
                        result = mob;
                }

                return result;
            };

            static MapObject* slowest(Mob* caster, BattleField* battleField, std::vector<MapObject*> candidates)
            {
                MapObject* result = nullptr;
                for(unsigned int i = 0; i < candidates.size(); i++)
                {
                    MapObject* mob = candidates.at(i);

                    if (result == nullptr)
                        result = mob;
                    else if (mob->speed() < result->speed())
                        result = mob;
                }

                return result;
            };


            // Verbs

            static void defend(MapObject* source, MapObject* target, int cost, int effect)
            {
                source->changeStamina(cost * -1);
                target->changeDefence(1 + (float(effect) / 100.0));
            };

            static void endanger(MapObject* source, MapObject* target, int cost, int effect)
            {
                source->changeStamina(cost * -1);
                target->changeDefence(1 - (float(effect) / 100.0));
            };

            static void ward(MapObject* source, MapObject* target, int cost, int effect)
            {
                source->changeStamina(cost * -1);
                target->changeResistance(1 + (float(effect) / 100.0));
            };

            static void expose(MapObject* source, MapObject* target, int cost, int effect)
            {
                source->changeStamina(cost * -1);
                target->changeResistance(1 - (float(effect) / 100.0));
            };

            static void weaken(MapObject* source, MapObject* target, int cost, int effect)
            {
                source->changeStamina(ceil(cost*-1));
                target->changeStamina(ceil(effect*-1));
            };

            static void strengthen(MapObject* source, MapObject* target, int cost, int effect)
            {
                source->changeStamina(ceil(cost*-1));
                target->changeStamina(ceil(effect));
            };

            static void hasten(MapObject* source, MapObject* target, int cost, int effect)
            {
                source->changeStamina(cost * -1);
                target->changeSpeed(1 + (float(effect) / 100.0));
            };

            static void slow(MapObject* source, MapObject* target, int cost, int effect)
            {
                source->changeStamina(cost * -1);
                target->changeSpeed(1 - (float(effect) / 100.0));
            };

            static void enhance(MapObject* source, MapObject* target, int cost, int effect)
            {
                source->changeStamina(cost * -1);
                target->changeSkill(1  + (float(effect) / 100.0));
            };

            static void impair(MapObject* source, MapObject* target, int cost, int effect)
            {
                source->changeStamina(cost * -1);
                target->changeSkill(1 - (float(effect) / 100.0));
            };
    };
}
#endif
