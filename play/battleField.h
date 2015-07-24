#ifndef BATTLEFIELD_H_INCLUDED
#define BATTLEFIELD_H_INCLUDED

#include "../mapObject.h"
#include <vector>
#include <queue>
#include "../globalConstants.h"

class GameMap;
class Mob;
namespace Magic {
class BattleField {
    public:
        BattleField(GameMap*);
        ~BattleField();
        BattleField(void){};
        std::vector<Mob*> mobs(void);
        std::vector<Mob*> pcs(void);
        std::vector<Mob*> hostiles(void);
        std::vector<Mob*> getDue(void);
        bool areAllied(const MapObject* one, const MapObject* other) const;
        bool isInCombat(bool);
        bool isInCombat(void) const;

        bool isVictory(void) const;
        bool isDefeat(void) const;

        void toBin(MapObject*);

    private:
        bool _isInCombat;
        std::vector<MapObject*> _rubbishBin;
        std::vector<Mob*> _pcs;
        std::vector<Mob*> _hostiles;
};
}

#endif
