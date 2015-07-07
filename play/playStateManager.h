#ifndef PLAYSTATEMANAGER_H_INCLUDED
#define PLAYSTATEMANAGER_H_INCLUDED

#include <vector>
#include "../stateManager.h"
#include "gameMap.h"
#include "mob.h"
#include <SDL2/SDL.h>
#include "../mapFileBlock.h"
#include "viewManagers.h"

    class PlayStateManager : public StateManager<Play::PlayState, Core::CoreState>
    {
        public:
            PlayStateManager(SDL_Renderer*, AssetCache*);
            ~PlayStateManager(void);
            Core::CoreState start(void);
            static const int CELL_WIDTH = 50;
            static const int CELL_HEIGHT = 50;
        private:
            // Set up
            GameMap* loadMap(void);
            std::vector<Mob> buildMobs(int, std::vector<MapCell>);
            void render(void);
            void renderTerrain();
            void renderMobs();
            void renderHud();
            std::vector<char> readFile(const char*);
            void writeMapFile(const char* fileName, const int width, const int height, const std::vector<MapFileBlock>* data);

            // Main Loop
            bool processMovementState(void);
            bool moveMob(Mob*, Core::InputPress);

            // other
            void exit(const Core::CoreState = Core::CoreState::Exit);

            // Properties
            SDL_Window* _window = NULL;
            ControlViewManager* _controlView = nullptr;
            StatsViewManager* _statsView = nullptr;
            ViewManager* _miniMapView = nullptr;
            MapViewManager* _mapView = nullptr;
            GameMap* _map = nullptr;
            int _combatGraceTime = 0;

            std::vector<MapFileBlock> tempMapFile();
    };
#endif
