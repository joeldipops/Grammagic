#ifndef MENUMANAGER_H_INCLUDED
#define MENUMANAGER_H_INCLUDE

#include "stateManager.h"
#include "play/mob.h"
#include "menuViewManager.h"
#include "globalConstants.h"
#include <SDL2/SDL.h>

namespace Play
{
    class MenuManager : public Core::StateManager<Play::MenuState, Play::PlayState>
    {
        public:
            MenuManager(SDL_Renderer*, AssetCache*);
            Play::PlayState start(Mob* pc);
            Play::PlayState start(void);

        private:
            bool moveCursor(Mob* pc, Core::InputPress input);
            bool processRuneCommand(Mob* pc);
            bool processSpellCommand(Mob* pc);
            MenuViewManager _viewManager;
            int _selectedSpellIndex;
            int _selectedRuneIndex;
    };
}

#endif

