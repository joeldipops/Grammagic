#ifndef BATTLEFIELD_H_INCLUDED
#define BATTLEFIELD_H_INCLUDED

#include <vector>
#include <queue>

#include "../mapObject.h"
#include "../globalConstants.h"
#include "combatable.h"

namespace Play
{
    class Mob;
    class GameMap;

    class BattleField {
        public:
            BattleField(GameMap*);
            ~BattleField();
            BattleField(void){};
            std::vector<Combatable*> combatants(void);
            std::vector<Mob*> pcs(void);
            std::vector<Mob*> hostiles(void);
            std::vector<Mob*> getDue(void);
            bool areAllied(const Combatable* one, const Combatable* other) const;
            bool isInCombat(bool);
            bool isInCombat(void) const;

            bool isVictory(void) const;
            bool isDefeat(void) const;

            void addToField(Combatable*, bool isPlayerAllied);
            void endCombat(void);

        private:
            bool _isInCombat;
            std::vector<Combatable*> _rubbishBin;
            std::vector<Mob*> _pcs;
            std::vector<Mob*> _hostiles;
            std::vector<Combatable*> _playerAllied;
            std::vector<Combatable*> _nonPlayerAllied;
            GameMap* _map;


    };
}

#endif
