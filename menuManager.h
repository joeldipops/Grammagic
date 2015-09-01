#ifndef MENUMANAGER_H_INCLUDED
#define MENUMANAGER_H_INCLUDE

#include <SDL2/SDL.h>

#include "stateManager.h"
#include "play/mob.h"
#include "menuViewManager.h"
#include "globalConstants.h"
#include "persistence/saveLoad.h"
#include "res/strings.h"
#include "util/utils.h"

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

            natural selectedSpellLength(PC*) const;
            bool moveCursor(Party& party, Core::InputPress input);

            // What happens when you hit ok when on various menu items.
            bool processMenuCommand(const Party& party);
            bool processMemberCommand(void);
            bool processRuneCommand(const Party& party);
            bool processSpellCommand(void);
            bool processComponentCommand(void);
            bool processReorderMemberCommand(Party& party);

            bool processCommand(Party& party);
            bool processCancel(void);

            MenuViewManager _viewManager;
            int _selectedMenuIndex;
            int _selectedMemberIndex;
            int _selectedSpellIndex;
            int _selectedRuneIndex;
            int _selectedComponentIndex;
            int _selectedPositionIndex;
            std::vector<MenuItem> _menu;
            std::string _message = "";
    };
}

#endif

