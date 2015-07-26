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

            static Combatable* self (Mob* caster, BattleField*)
            {
                return (Combatable*) caster;
            };


            // General Nouns

            static std::vector<Combatable*> enemies(Mob* caster, BattleField* battleField)
            {
                std::vector<Combatable*> result;
                std::vector<Mob*> candidates;
                if (caster->type() == MobType::PlayerCharacter)
                    candidates = battleField->hostiles();
                else
                    candidates = battleField->pcs();

                for (Mob* m : candidates)
                {
                    result.push_back((Combatable*) m);
                }

                return result;
            }


            // Adjectives
            static Combatable* all(Mob* caster, BattleField* field, std::vector<Combatable*> candidates)
            {
                TargetAll* result = new TargetAll(candidates);
                field->toBin(result);
                return result;
            }

            static Combatable* exposed(Mob* caster, BattleField* battleField, std::vector<Combatable*> candidates)
            {
                Combatable* result = nullptr;
                for(unsigned int i = 0; i < candidates.size(); i++)
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
                for(unsigned int i = 0; i < candidates.size(); i++)
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
                for(unsigned int i = 0; i < candidates.size(); i++)
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
                for(unsigned int i = 0; i < candidates.size(); i++)
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
                for(unsigned int i = 0; i < candidates.size(); i++)
                {
                    Combatable* mob = candidates.at(i);

                    if (result == nullptr)
                        result = mob;
                    else if (mob->stamina() < result->stamina())
                        result = mob;
                }

                return result;
            };

            static Combatable* sickest(Mob* caster, BattleField* battleField, std::vector<Combatable*> candidates)
            {
                Combatable* result = nullptr;
                for(unsigned int i = 0; i < candidates.size(); i++)
                {
                    Combatable* mob = candidates.at(i);

                    if (result == nullptr)
                        result = mob;
                    else if (mob->stamina() > result->stamina())
                        result = mob;
                }

                return result;
            };


            static Combatable* strongest(Mob* caster, BattleField* battleField, std::vector<Combatable*> candidates)
            {
                Combatable* result = nullptr;
                for(unsigned int i = 0; i < candidates.size(); i++)
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
                for(unsigned int i = 0; i < candidates.size(); i++)
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
                for(unsigned int i = 0; i < candidates.size(); i++)
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
                for(unsigned int i = 0; i < candidates.size(); i++)
                {
                    Combatable* mob = candidates.at(i);

                    if (result == nullptr)
                        result = mob;
                    else if (mob->speed() < result->speed())
                        result = mob;
                }

                return result;
            };


            // Verbs

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
    };
}
#endif
