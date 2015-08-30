#ifndef BATTLECOMMANDS_H_INCLUDED
#define BATTLECOMMANDS_H_INCLUDED

#include "../play/battleField.h"
#include "../play/mapObject.h"
#include "../magic/properNoun.h"
#include "../magic/verb.h"
#include "../magic/modifier.h"
#include "../magic/adjective.h"
#include "../magic/noun.h"
#include "../play/mob.h"
#include "../magic/targetAll.h"
#include "../util/utils.h"

namespace Magic
{
}

namespace Play { class Enemy; }

using namespace Play;
namespace Templates
{

    struct Commands
    {
        // Non spell commands
        static int FLEE(Command*, Mob*, BattleField* field);
        static int ATTACK(Command* command, Mob* actor, BattleField* field);

        static std::vector<Word*> allCommands;
        static ProperNoun CASTER;
        static Noun ENEMY;
        static Noun ALLY; // Excludes the caster.
        static Noun MEMBER; // Includes the caster.
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
    };
}
#endif
