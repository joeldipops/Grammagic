#ifndef MENUVIEWMANAGER_H_INCLUDED
#define MENUVIEWMANAGER_H_INCLUDED

#include "util/utils.h"
#include "viewManager.h"
#include "magic/battleCommands.h"
#include "rune.h"
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
            int SelectedSpellIndex;
            int SelectedComponentIndex;
            int SelectedRuneIndex;
    };

    class MenuViewManager : public ViewManager
    {
        public:
            MenuViewManager(){};
            MenuViewManager(SDL_Renderer*, SDL_Rect, AssetCache*);
            void render(const Party&, const MenuViewModel&, std::string* = nullptr);

        private:
            SDL_Rect _spellsVp;
            SDL_Rect _runesVp;
            SDL_Rect _mainVp;

            void renderSpells(const PC&, int, int);
            void renderRunes(const PC&, int);

            static const SDL_Rect _menuControl;
            static const int borderWidth = 5;
            static const int cursorXOffset;
            static const int cursorYOffset;
            static const int marginLeft = 40;
    };
}

#endif
