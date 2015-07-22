#include "menuManager.h"

using namespace Play;

const MenuItem MenuManager::MAGIC = MenuItem(Strings::Magic);
const MenuItem MenuManager::SAVE = MenuItem(Strings::Save);

MenuManager::MenuManager(SDL_Renderer* r, AssetCache* a)
    : StateManager(r, a)
{
    _viewManager = MenuViewManager(r, SDL_Rect {0, 0, WIDTH, HEIGHT }, a);
    _selectedSpellIndex = -1;
    _selectedRuneIndex = -1;
    _selectedComponentIndex = -1;
    _menu = std::vector<MenuItem> { MAGIC, SAVE };
}

Play::PlayState MenuManager::start(PC* pc)
{
    bool rerender = true;
    state(MenuState::SelectMenu);
    result(PlayState::Menu);
    _selectedSpellIndex = -1;
    _selectedComponentIndex = -1;
    _selectedRuneIndex = -1;
    while(result() == PlayState::Menu)
    {
        if (rerender)
            _viewManager.render(pc, _menu, MainMenuItem(_selectedMenuIndex), _selectedSpellIndex, _selectedComponentIndex, _selectedRuneIndex);
        else
            Util::Util::sleep(50);

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

/**
 * @param pc who has a list of spells.
 * @return The number of components in the currently selected spell.
 */
int MenuManager::selectedSpellLength(PC* pc) const
{
    // We have only just started writing this spell.
    if (int(pc->spells()->size()) <= _selectedSpellIndex)
        return 0;

    return (int) pc->spells()->at(_selectedSpellIndex).spell()->components().size();
}


bool MenuManager::moveCursor(PC* pc, Core::InputPress input)
{
    int result = 0;
    int index = 0;
    int itemCount = 0;
    int columnItemCount = 0;
    switch(state())
    {
        case MenuState::SelectMenu:
            itemCount = _menu.size();
            index = _selectedMenuIndex;
            columnItemCount = 100;
            break;
        case MenuState::SelectSpell:
            itemCount = pc->spellSlots() > int(pc->spells()->size())
                ? pc->spells()->size() + 1
                : pc->spellSlots();
            index = _selectedSpellIndex;
            columnItemCount = 1;
            break;
        case MenuState::SelectRune:
            index = _selectedRuneIndex;
            itemCount = Commands::allCommands.size() + 1;
            columnItemCount = _viewManager.menuItemsPerColumn();
            break;
        case MenuState::SelectComponent:
            index = _selectedComponentIndex;
            itemCount = pc->runeSlots() > selectedSpellLength(pc)
                ? selectedSpellLength(pc) + 1
                : pc->runeSlots();
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
            case MenuState::SelectMenu:
                _selectedMenuIndex = result;
                return true;
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
            _selectedSpellIndex = -1;
            state(MenuState::SelectMenu);
            return true;
        case MenuState::SelectMenu:
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
        case MenuState::SelectMenu:
            return processMenuCommand(pc);
        case MenuState::SelectSpell:
            return processSpellCommand(pc);
        case MenuState::SelectRune:
            return processRuneCommand(pc);
        case MenuState::SelectComponent:
            return processComponentCommand(pc);
    }
    return false;
}

bool MenuManager::processMenuCommand(PC* pc)
{
    MenuItem item = _menu.at(_selectedMenuIndex);
    if (item.equals(MAGIC))
    {
        _selectedSpellIndex = 0;
        state(MenuState::SelectSpell);
        return true;
    }
    else if (item.equals(SAVE))
    {
        Persistence::SaveLoad io = Persistence::SaveLoad(SAVE_FILE);
        io.save(*pc);
        return true;
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
        pc->spells()->push_back(Command("", Spell(std::vector<Word*>(0))));

    Spell* workingSpell = pc->spells()->at(_selectedSpellIndex).spell();

    if (_selectedRuneIndex == 0)
    {
        workingSpell->removeComponent(_selectedComponentIndex);
        return true;
    }

    workingSpell->component(_selectedComponentIndex, Commands::allCommands.at(_selectedRuneIndex - 1));
    if (_selectedComponentIndex + 1 < pc->runeSlots())
        _selectedComponentIndex++;
    return true;
}


Play::PlayState MenuManager::start(void)
{
    return Play::PlayState::Exit;
}
