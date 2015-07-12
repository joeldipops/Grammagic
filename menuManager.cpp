#include "menuManager.h"

using namespace Play;

MenuManager::MenuManager(SDL_Renderer* r, AssetCache* a)
    : StateManager(r, a)
{
    _viewManager = MenuViewManager(r, SDL_Rect {0, 0, WIDTH, HEIGHT }, a);
    _selectedSpellIndex = -1;
    _selectedRuneIndex = -1;
}

Play::PlayState MenuManager::start(Mob* pc)
{
    bool rerender = true;
    state(MenuState::SelectSpell);
    result(PlayState::Menu);
    _selectedSpellIndex = 0;
    _selectedRuneIndex = -1;
    while(result() == PlayState::Menu)
    {
        if (rerender)
            _viewManager.render(pc, _selectedSpellIndex, _selectedRuneIndex);

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
                    {
                        result(Play::PlayState::Movement);
                        continue;
                    }

                    switch(event.key.keysym.sym)
                    {
                        case SDLK_w:
                            rerender |= moveCursor(pc, Core::InputPress::UP);
                            break;
                        case SDLK_a:
                            rerender |= moveCursor(pc, Core::InputPress::LEFT);
                            break;
                        case SDLK_s:
                            rerender |= moveCursor(pc, Core::InputPress::DOWN);
                            break;
                        case SDLK_d:
                            rerender |= moveCursor(pc, Core::InputPress::RIGHT);
                            break;
                        case SDLK_SPACE:
                            if (state() == MenuState::SelectSpell)
                                rerender |= processSpellCommand(pc);
                            else if (state() == MenuState::SelectRune)
                                rerender |= processRuneCommand(pc);
                            break;
                    }

                    break;
                default:
                    rerender |= false;
                    break;
            }
        }
    }
    return result();
}

bool MenuManager::moveCursor(Mob* pc, Core::InputPress input)
{
    int result = 0;
    int index = 0;
    int itemCount = 0;
    int columnItemCount = 0;
    switch(state())
    {
        case MenuState::SelectSpell:
            for (Command c : pc->commands())
            {
                if (c.components().size() > 0)
                    itemCount++;
            }
            index = _selectedSpellIndex;
            columnItemCount = 1;
            break;
        case MenuState::SelectRune:
            index = _selectedRuneIndex;
            itemCount = Commands::allCommands.size();
            columnItemCount = _viewManager.menuItemsPerColumn();
            break;
        default:
            return false;
    }

    result = StateManager::moveCursor(input, index, itemCount, columnItemCount);

    if (result != index)
    {
        switch(state())
        {
            case MenuState::SelectSpell:
                _selectedSpellIndex = result;
                return true;
            case MenuState::SelectRune:
                _selectedRuneIndex = result;
                return true;
            default: return false;
        }
    }

    return false;
}

bool MenuManager::processSpellCommand(Mob* pc)
{
    _selectedRuneIndex = 0;
    state(MenuState::SelectRune);
    return true;
}

bool MenuManager::processRuneCommand(Mob* pc)
{
    _selectedRuneIndex = -1;
    state(MenuState::SelectSpell);
    return true;
}


Play::PlayState MenuManager::start(void)
{
    return Play::PlayState::Exit;
}
