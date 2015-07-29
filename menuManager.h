#ifndef MENUMANAGER_H_INCLUDED
#define MENUMANAGER_H_INCLUDE

#include "stateManager.h"
#include "play/mob.h"
#include "menuViewManager.h"
#include "globalConstants.h"
#include "persistence/saveLoad.h"
#include "res/strings.h"
#include "util/utils.h"

#include <SDL2/SDL.h>

namespace Play
{
    class MenuManager : public Core::StateManager<Play::MenuState, Play::PlayState>
    {
        public:
            MenuManager(SDL_Renderer*, AssetCache*);
            Play::PlayState start(Party& party);
            Play::PlayState start(void);

        private:
            static const MenuItem MAGIC;
            static const MenuItem SAVE;
            static const MenuItem PARTY;

            int selectedSpellLength(PC*) const;
            bool moveCursor(Party& party, Core::InputPress input);
            bool processMenuCommand(const Party& party);
            bool processMemberCommand(void);
            bool processRuneCommand(const Party& party);
            bool processSpellCommand(void);
            bool processComponentCommand(void);
            bool processCommand(Party& party);
            bool processCancel(void);

            MenuViewManager _viewManager;
            int _selectedMenuIndex;
            int _selectedMemberIndex;
            int _selectedSpellIndex;
            int _selectedRuneIndex;
            int _selectedComponentIndex;
            std::vector<MenuItem> _menu;
            std::string _message = "";
    };
}

#endif

