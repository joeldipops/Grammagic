#ifndef MENUVIEWMANAGER_H_INCLUDED
#define MENUVIEWMANAGER_H_INCLUDED

#include "util/utils.h"
#include "viewManager.h"
#include "magic/battleCommands.h"
#include "rune.h"

class MenuViewManager : public ViewManager
{
    public:
        MenuViewManager(){};
        MenuViewManager(SDL_Renderer*, SDL_Rect, AssetCache*);
        void render(Mob* pc);

    private:
        SDL_Rect _spellsVp;
        SDL_Rect _runesVp;
        void renderSpells(Mob* pc);
        void renderRunes(Mob* pc);

};

#endif
