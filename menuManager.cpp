#include "menuManager.h"

using namespace Play;

const MenuItem MenuManager::MAGIC = MenuItem(Strings::Magic);
const MenuItem MenuManager::SAVE = MenuItem(Strings::Save);
const MenuItem MenuManager::PARTY = MenuItem(Strings::Party);

MenuManager::MenuManager(SDL_Renderer* r, AssetCache* a)
    : StateManager(r, a)
{
    _viewManager = MenuViewManager(r, SDL_Rect {0, 0, WIDTH, HEIGHT }, a);
    _selectedSpellIndex = -1;
    _selectedRuneIndex = -1;
    _selectedComponentIndex = -1;
    _selectedMemberIndex = -1;
    _menu = std::vector<MenuItem> { MAGIC, SAVE, PARTY };
}

Play::PlayState MenuManager::start(Party& party)
{
    bool rerender = true;
    _selectedMenuIndex = 0;
    state(MenuState::SelectMenu);
    result(PlayState::Menu);

    _selectedSpellIndex = -1;
    _selectedComponentIndex = -1;
    _selectedRuneIndex = -1;
    _selectedMemberIndex = -1;
    while(result() == PlayState::Menu)
    {
        if (rerender)
        {
            MenuViewModel vm
            {
                _menu, // MenuItems
                MainMenuItem(_selectedMenuIndex), // SelectedMenuItem
                state(),
                _selectedMemberIndex,
                _selectedSpellIndex, // SelectedSpellIndex
                _selectedComponentIndex, // SelectedComponentIndex
                _selectedRuneIndex, // SelectedRuneIndex
            };
            _viewManager.render(party, vm, _message.length() > 0 ? &_message : nullptr);
        }

        else
            Util::sleep(50);

        rerender = false;

        SDL_Event event;

        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    return Play::PlayState::Exit;
                case SDL_KEYDOWN:
                    _message = "";
                    if (event.key.keysym.sym == SDLK_RETURN)
                    {
                        result(Play::PlayState::Movement);
                        continue;
                    }

                    switch(event.key.keysym.sym)
                    {
                        case SDLK_w:
                            rerender |= moveCursor(party, Core::InputPress::UP);
                            break;
                        case SDLK_a:
                            rerender |= moveCursor(party, Core::InputPress::LEFT);
                            break;
                        case SDLK_s:
                            rerender |= moveCursor(party, Core::InputPress::DOWN);
                            break;
                        case SDLK_d:
                            rerender |= moveCursor(party, Core::InputPress::RIGHT);
                            break;
                        case SDLK_LEFTBRACKET:
                            rerender |= processCancel();
                            break;
                        case SDLK_RIGHTBRACKET:
                            rerender |= processCommand(party);
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
    for (Command* cmd : party.leader()->commands())
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


bool MenuManager::moveCursor(Party& party, Core::InputPress input)
{
    int result = 0;
    int index = 0;
    unsigned int itemCount = 0;
    int columnItemCount = 0;
    switch(state())
    {
        case MenuState::SelectMenu:
            itemCount = _menu.size();
            index = _selectedMenuIndex;
            columnItemCount = itemCount;
            break;
        case MenuState::SelectMember:
        case MenuState::ReorderMember:
            itemCount = party.members().size();
            columnItemCount = itemCount;
            index = _selectedMemberIndex;
            break;
        case MenuState::SelectSpell:
            itemCount = party.leader()->spellSlots() > int(party.leader()->spells()->size())
                ? party.leader()->spells()->size() + 1
                : party.leader()->spellSlots();
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
            itemCount = party.leader()->runeSlots() > selectedSpellLength(party.leader())
                ? selectedSpellLength(party.leader()) + 1
                : party.leader()->runeSlots();
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
            case MenuState::SelectMember:
                _selectedMemberIndex = result;
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
            state(MenuState::SelectMember);
            return true;
        case MenuState::SelectMember:
            _selectedMemberIndex = -1;
            state(MenuState::SelectMenu);
            return true;
        case MenuState::SelectMenu:
            result(PlayState::Movement);
            return true;
        default:
            return false;
    }
}

bool MenuManager::processCommand(Party& party)
{
    switch(state())
    {
        case MenuState::SelectMenu:
            return processMenuCommand(party);
        case MenuState::SelectSpell:
            return processSpellCommand();
        case MenuState::SelectRune:
            return processRuneCommand(party);
        case MenuState::SelectComponent:
            return processComponentCommand();
        case MenuState::SelectMember:
            return processMemberCommand();
        case MenuState::ReorderMember:
            return false;
    }
    return false;
}

bool MenuManager::processMenuCommand(const Party& party)
{
    MenuItem item = _menu.at(_selectedMenuIndex);
    if (item.equals(MAGIC))
    {
        _selectedMemberIndex = 0;
        state(MenuState::SelectMember);
        return true;
    }
    else if (item.equals(SAVE))
    {
        Persistence::SaveLoad io = Persistence::SaveLoad(SAVE_FILE);
        io.save(party);
        _message = Strings::SaveComplete;
        return true;
    }
    else if (item.equals(PARTY))
    {
        //state(MenuState::SelectPc);
    }

    return false;
}

bool MenuManager::processMemberCommand(void)
{
    _selectedSpellIndex = 0;
    state(MenuState::SelectSpell);
    return true;
}

bool MenuManager::processComponentCommand(void)
{
    _selectedRuneIndex = 0;
    state(MenuState::SelectRune);
    return true;
}

bool MenuManager::processSpellCommand(void)
{
    _selectedComponentIndex = 0;
    state(MenuState::SelectComponent);
    return true;
}

bool MenuManager::processRuneCommand(const Party& party)
{
    PC* pc = party.members().at(_selectedMemberIndex);
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
