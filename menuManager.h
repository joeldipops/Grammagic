#ifndef MENUMANAGER_H_INCLUDED
#define MENUMANAGER_H_INCLUDE

#include "stateManager.h"
#include "play/mob.h"
#include "menuViewManager.h"
#include "globalConstants.h"
#include <SDL2/SDL.h>

class MenuManager : public StateManager<Play::PlayState, Play::PlayState>
{
    public:
        MenuManager(SDL_Renderer*, AssetCache*);
        Play::PlayState start(Mob* pc);
        Play::PlayState start(void);

    protected:
        virtual Play::PlayState result(void) const;

    private:
        bool moveCursor(Mob* mob, Core::InputPress input);
        MenuViewManager _viewManager;

};

#endif
