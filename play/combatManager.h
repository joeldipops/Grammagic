#ifndef COMBATMANAGER_H_INCLUDED
#define COMBATMANAGER_H_INCLUDED

#include "../stateManager.h"
#include "../play/viewManagers.h"
#include "../play/gameMap.h"
#include "battleField.h"
#include "../screenViewContainer.h"
#include "../play/enemy.h"

namespace Magic
{
    class CombatManager : public Core::StateManager<Play::PlayState, Play::PlayState>
    {
        public:
            CombatManager(SDL_Renderer*, AssetCache*, ScreenViewContainer);
            ~CombatManager(void);

            Play::PlayState start(GameMap*);
            Play::PlayState start(void);

        protected:
            virtual Play::PlayState result(void) const;
            virtual Play::PlayState state(Play::PlayState);

        private:

            bool processCommand(Mob*, BattleField*);
            bool processPcTurn(Mob*, BattleField*, std::vector<SDL_Event>*);
            bool processHostileTurn(Enemy*, BattleField*);
            void render(void);
            bool moveCursor(Mob* mob, Core::InputPress input);

            // views
            ControlViewManager* _controlView;
            StatsViewManager* _statsView;
            MapViewManager* _mapView;
            MiniMapViewManager* _miniMapView;

            BattleField* _field = nullptr;
            GameMap* _map = nullptr;
            int _selectedMemberIndex = -1;
    };
}

#endif


