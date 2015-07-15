#ifndef MENUVIEWMANAGER_H_INCLUDED
#define MENUVIEWMANAGER_H_INCLUDED

#include "util/utils.h"
#include "viewManager.h"
#include "magic/battleCommands.h"
#include "rune.h"
#include "play/pc.h"

class MenuViewManager : public ViewManager
{
    public:
        MenuViewManager(){};
        MenuViewManager(SDL_Renderer*, SDL_Rect, AssetCache*);
        void render(PC* pc, int, int, int);

    private:
        SDL_Rect _spellsVp;
        SDL_Rect _runesVp;
        void renderSpells(PC* pc);
        void renderRunes(PC* pc);

        static const int marginLeft = 50;

};

#endif
