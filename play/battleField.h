#ifndef BATTLEFIELD_H_INCLUDED
#define BATTLEFIELD_H_INCLUDED

#include <vector>
#include <queue>

#include "../magic/spellContext.h"
#include "mapObject.h"
#include "../globalConstants.h"
#include "combatable.h"

namespace Play
{
    class Mob;
    class GameMap;

    class BattleField : public Magic::SpellContext
    {
        public:
            BattleField(void) {};

            BattleField(GameMap*);
            ~BattleField();
            std::vector<Mob*> getDue(void);

            bool isVictory(void) const;
            bool isDefeat(void) const;

            void buryTheDead(void);
            void endCombat(void);

        private:
            GameMap* _map = nullptr;
    };
}

#endif
