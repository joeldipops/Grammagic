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
            Play::PlayState start(PC* pc);
            Play::PlayState start(void);

        private:
            int selectedSpellLength(PC*) const;
            bool moveCursor(PC* pc, Core::InputPress input);
            bool processRuneCommand(PC* pc);
            bool processSpellCommand(PC* pc);
            bool processComponentCommand(PC* pc);
            bool processCommand(PC* pc);
            bool processCancel(void);
            MenuViewManager _viewManager;
            int _selectedSpellIndex;
            int _selectedRuneIndex;
            int _selectedComponentIndex;
    };
}

#endif

