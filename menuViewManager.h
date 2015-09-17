#ifndef MENUVIEWMANAGER_H_INCLUDED
#define MENUVIEWMANAGER_H_INCLUDED

#include "util/utils.h"
#include "viewManager.h"
#include "res/battleCommands.h"
#include "play/pc.h"
#include "globalConstants.h"
#include "play/party.h"

namespace Play
{
    struct MenuViewModel
    {
        public:
            std::vector<MenuItem> MenuItems;
            MainMenuItem SelectedMenuItem;
            MenuState state;
            int SelectedPCIndex;
            int SelectedSpellIndex;
            int SelectedComponentIndex;
            int SelectedRuneIndex;
            int SelectedPositionIndex;
    };

    class MenuViewManager : public ViewManager
    {
        public:
            MenuViewManager(){};
            MenuViewManager(SDL_Renderer*, SDL_Rect, AssetCache*);
            void render(const Party&, const MenuViewModel&, const std::string* = nullptr);

        private:
            SDL_Rect _spellsVp;
            SDL_Rect _runesVp;
            SDL_Rect _mainVp;
            SDL_Rect _partyVp;

            void renderSpells(const PC&, int, int);
            void renderRunes(const Party&, int);
            void renderPCs(const Party&, int, int = -1);

            static const SDL_Rect _menuControl;
            static const SDL_Rect _runeControl;
            static const int borderWidth = 5;
            static const int cursorXOffset;
            static const int cursorYOffset;
            static const int marginLeft = 40;
    };
}

#endif
