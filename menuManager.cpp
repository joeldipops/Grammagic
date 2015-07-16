#include "menuManager.h"

using namespace Play;

MenuManager::MenuManager(SDL_Renderer* r, AssetCache* a)
    : StateManager(r, a)
{
    _viewManager = MenuViewManager(r, SDL_Rect {0, 0, WIDTH, HEIGHT }, a);
    _selectedSpellIndex = -1;
    _selectedRuneIndex = -1;
    _selectedComponentIndex = -1;
}

Play::PlayState MenuManager::start(PC* pc)
{
    bool rerender = true;
    state(MenuState::SelectSpell);
    result(PlayState::Menu);
    _selectedSpellIndex = 0;
    _selectedComponentIndex = -1;
    _selectedRuneIndex = -1;
    while(result() == PlayState::Menu)
    {
        if (rerender) {
            _viewManager.render(pc, _selectedSpellIndex, _selectedComponentIndex, _selectedRuneIndex);
        }

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
                        case SDLK_LEFTBRACKET:
                            rerender |= processCancel();
                            break;
                        case SDLK_RIGHTBRACKET:
                            rerender |= processCommand(pc);
                            break;
                    }

                    break;
                default:
                    rerender |= false;
                    break;
            }
        }
    }

    // Resolve all spells.
    for (Command* cmd : pc->commands())
    {
        cmd->spell()->resolve();
    }

    return result();
}

bool MenuManager::moveCursor(PC* pc, Core::InputPress input)
{
    int result = 0;
    int index = 0;
    int itemCount = 0;
    int columnItemCount = 0;
    switch(state())
    {
        case MenuState::SelectSpell:
            itemCount = pc->spellSlots();
            index = _selectedSpellIndex;
            columnItemCount = 1;
            break;
        case MenuState::SelectRune:
            index = _selectedRuneIndex;
            itemCount = Commands::allCommands.size();
            columnItemCount = _viewManager.menuItemsPerColumn();
            break;
        case MenuState::SelectComponent:
            index = _selectedComponentIndex;
            itemCount = pc->runeSlots();
            columnItemCount = 1;
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
            case MenuState::SelectComponent:
                _selectedComponentIndex = result;
                return true;
            default: return false;
        }
    }

    return false;
}

bool MenuManager::processCancel(void)
{
    switch(state())
    {
        case MenuState::SelectRune:
            _selectedRuneIndex = -1;
            state(MenuState::SelectComponent);
            return true;
        case MenuState::SelectComponent:
            _selectedComponentIndex = -1;
            state(MenuState::SelectSpell);
            return true;
        case MenuState::SelectSpell:
            result(PlayState::Movement);
            return true;
        default:
            return false;
    }
}

bool MenuManager::processCommand(PC* pc)
{
    switch(state())
    {
        case MenuState::SelectSpell:
            return processSpellCommand(pc);
        case MenuState::SelectRune:
            return processRuneCommand(pc);
        case MenuState::SelectComponent:
            return processComponentCommand(pc);
    }
    return false;
}

bool MenuManager::processComponentCommand(PC* pc)
{
    _selectedRuneIndex = 0;
    state(MenuState::SelectRune);
    return true;
}

bool MenuManager::processSpellCommand(PC* pc)
{
    _selectedComponentIndex = 0;
    state(MenuState::SelectComponent);
    return true;
}

bool MenuManager::processRuneCommand(PC* pc)
{
    if (int(pc->spells()->size()) <= _selectedSpellIndex)
        pc->spells()->push_back(Command("abc", Spell(std::vector<Word*>(0))));

    Spell* workingSpell = pc->spells()->at(_selectedSpellIndex).spell();
    workingSpell->component(_selectedComponentIndex, Commands::allCommands.at(_selectedRuneIndex));
    if (_selectedComponentIndex < pc->runeSlots())
        _selectedComponentIndex++;
    return true;
}


Play::PlayState MenuManager::start(void)
{
    return Play::PlayState::Exit;
}
