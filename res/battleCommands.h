#ifndef BATTLECOMMANDS_H_INCLUDED
#define BATTLECOMMANDS_H_INCLUDED

#include "../play/battleField.h"
#include "../play/mapObject.h"
#include "../magic/verb.h"
#include "../magic/modifier.h"
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
    };
}
#endif
