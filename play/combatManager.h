#ifndef COMBATMANAGER_H_INCLUDED
#define COMBATMANAGER_H_INCLUDED

#include "../stateManager.h"
#include "../play/gameMap.h"
#include "battleField.h"
#include "../view/screenViewContainer.h"
#include "../play/enemy.h"

namespace View
{
    class ControlViewManager;
    class StatsViewManager;
    class MapViewManager;
    class MiniMapViewManager;
    class VictoryViewManager;
}

namespace Magic
{
    class CombatManager : public Core::StateManager<Play::PlayState, Play::PlayState>
    {
        public:
            CombatManager(SDL_Renderer*, AssetCache*, View::ScreenViewContainer);
            ~CombatManager(void);

            Play::PlayState start(GameMap*);
            Play::PlayState start(void);

        protected:
            using StateManager::result;
            using StateManager::state;
            virtual Play::PlayState result(void) const;
            virtual Play::PlayState state(Play::PlayState);

        private:

            bool processCommand(Mob*, BattleField&);
            bool processPcTurn(Mob*, BattleField&, std::vector<SDL_Event>*);
            bool processHostileTurn(Enemy*, BattleField&);
            void render(void);
            bool moveCursor(Mob* mob, Core::InputPress input);
            void buryTheDead(void);
            void endCombat(void);

            // views
            View::ControlViewManager* _controlView = nullptr;
            View::StatsViewManager* _statsView = nullptr;
            View::MapViewManager* _mapView = nullptr;
            View::MiniMapViewManager* _miniMapView = nullptr;
            View::VictoryViewManager* _victoryView = nullptr;

            BattleField _field;
            GameMap* _map = nullptr;
            int _selectedMemberIndex = -1;
    };
}

#endif


