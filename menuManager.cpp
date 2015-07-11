#include "menuManager.h"

MenuManager::MenuManager(SDL_Renderer* r, AssetCache* a)
    : StateManager(r, a)
{
    _viewManager = MenuViewManager(r, SDL_Rect {0, 0, WIDTH, HEIGHT }, a);
    state(Play::PlayState::Menu);
}

Play::PlayState MenuManager::start(Mob* pc)
{
    bool rerender = true;
    while(state() == Play::PlayState::Menu)
    {
        if (rerender)
            _viewManager.render(pc);

        rerender = false;

        SDL_Event event;

        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    return Play::PlayState::Exit;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_RETURN)
                        return Play::PlayState::Movement;
                    break;
                default:
                    rerender |= false;
                    break;
            }
        }
    }
    return result();
}

Play::PlayState MenuManager::start(void)
{
    return Play::PlayState::Exit;
}

Play::PlayState MenuManager::result(void) const
{
    return StateManager::state();
}
