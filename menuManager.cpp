#include "menuManager.h"

using namespace Play;

using namespace Resources;
const MenuItem MenuManager::MAGIC = MenuItem(Strings::Magic);
const MenuItem MenuManager::SAVE = MenuItem(Strings::Save);
const MenuItem MenuManager::PARTY = MenuItem(Strings::Party);


MenuManager::MenuManager(SDL_Renderer* r, AssetCache* a)
    : StateManager(r, a)
{
    _viewManager = View::MenuViewManager(r, SDL_Rect {0, 0, WIDTH, HEIGHT }, a);
    _selectedSpellIndex = -1;
    _selectedRuneIndex = -1;
    _selectedComponentIndex = -1;
    _selectedMemberIndex = -1;
    _selectedPositionIndex = -1;
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
            View::MenuViewModel vm
            {
                _menu, // MenuItems
                MainMenuItem(_selectedMenuIndex), // SelectedMenuItem
                state(),
                _selectedMemberIndex,
                _selectedSpellIndex, // SelectedSpellIndex
                _selectedComponentIndex, // SelectedComponentIndex
                _selectedRuneIndex, // SelectedRuneIndex
                _selectedPositionIndex

            };
            _viewManager.render(party, vm, _message.length() > 0 ? &_message : nullptr);
        }
        else
            Util::sleep(50);

        rerender = false;

        Event myEvent;
        while(eventManager.pollEvent(&myEvent))
        {
            SDL_Event event = *myEvent.InnerEvent;
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
                            rerender |= processCancel(party);
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

    return result();
}

/**
 * @param pc who has a list of spells.
 * @return The number of components in the currently selected spell.
 */
natural MenuManager::selectedSpellLength(PC* pc) const
{
    // We have only just started writing this spell.
    if (int(pc->spells().size()) <= _selectedSpellIndex)
        return 0;

    return pc->spells().at(_selectedSpellIndex)->components().size();
}

bool MenuManager::moveCursorSideways(Party& party, Core::InputPress input)
{
    if (state() != SelectSpell)
        return false;

    // This is all select spell state focused for now.
    std::vector<Command*> spells = party.memberAt(_selectedMemberIndex)->spells();
    if ((_selectedSpellIndex >= 0 && spells.size() <= natural(_selectedSpellIndex)) || !spells.at(_selectedSpellIndex)->isValid())
        return false;

    int* index = &_selectedComponentIndex;
    int min = -2;
    int max = -1;
    switch(input)
    {
        case Core::InputPress::LEFT:
            *index = *index > min ? *index - 1 : max;
            break;
        case Core::InputPress::RIGHT:
            *index = *index < max ? *index + 1 : min;
            break;
        case Core::InputPress::DOWN:
        case Core::InputPress::UP:
        default:
            return false;
    }

    return true;
}

bool MenuManager::moveCursor(Party& party, Core::InputPress input)
{
    if (state() == MenuState::SelectSpell && in(input, Core::InputPress::LEFT, Core::InputPress::RIGHT))
        return moveCursorSideways(party, input);

    int result = 0;
    int index = 0;
    natural itemCount = 0;
    int columnItemCount = 0;
    int* indexToUpdate = nullptr;
    switch(state())
    {
        case MenuState::SelectMenu:
            itemCount = _menu.size();
            index = _selectedMenuIndex;
            indexToUpdate = &_selectedMenuIndex;
            columnItemCount = itemCount;
            break;
        case MenuState::SelectMember:
            itemCount = party.members().size();
            columnItemCount = itemCount;
            index = _selectedMemberIndex;
            indexToUpdate = &_selectedMemberIndex;
            break;
        case MenuState::ReorderMember:
            itemCount = party.members().size();
            columnItemCount = itemCount;
            index = _selectedPositionIndex;
            indexToUpdate = &_selectedPositionIndex;
            break;
        case MenuState::SelectSpell: {
            PC* member = party.memberAt(_selectedMemberIndex);
            itemCount = member->spellSlots() > member->spells().size()
                ? member->spells().size() + 1
                : member->spellSlots();
            index = _selectedSpellIndex;
            indexToUpdate = &_selectedSpellIndex;
            columnItemCount = 1;
            break;
        }
        case MenuState::SelectRune:
            index = _selectedRuneIndex;
            indexToUpdate = &_selectedRuneIndex;
            itemCount = party.runeCollection().size() + 1;
            columnItemCount = _viewManager.menuItemsPerColumn();
            break;
        case MenuState::SelectComponent: {
            PC* member = party.memberAt(_selectedMemberIndex);
            index = _selectedComponentIndex;
            indexToUpdate = &_selectedComponentIndex;
            itemCount = member->runeSlots() > selectedSpellLength(member)
                ? selectedSpellLength(member) + 1
                : member->runeSlots();
            columnItemCount = 1;
            break;
        }
        default:
            return false;
    }

    result = StateManager::moveCursor(input, index, itemCount, columnItemCount);

    if (result != index && indexToUpdate != nullptr)
    {
        *indexToUpdate = result;
        return true;
    }

    return false;
}

/**
 * Moves to the previous step in the menu tree.
 * @param Party should you wish to perform any operations on them.
 */
bool MenuManager::processCancel(Party& party)
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
            party.memberAt(_selectedMemberIndex)->cleanUpSpellList();
            _selectedSpellIndex = -1;
            state(MenuState::SelectMember);
            return true;
        case MenuState::ReorderMember:
            _selectedPositionIndex = -1;
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
            return processSpellCommand(party);
        case MenuState::SelectRune:
            return processRuneCommand(party);
        case MenuState::SelectComponent:
            return processComponentCommand();
        case MenuState::SelectMember:
            return processMemberCommand();
        case MenuState::ReorderMember:
            return processReorderMemberCommand(party);
    }
    return false;
}

bool MenuManager::processMenuCommand(const Party& party)
{
    switch(MainMenuItem(_selectedMenuIndex))
    {
        case MainMenuItem::PartySelected:
        case MainMenuItem::MagicSelected:
            _selectedMemberIndex = 0;
            state(MenuState::SelectMember);
            return true;
        case MainMenuItem::SaveSelected: {
            Persistence::SaveLoad io = Persistence::SaveLoad(SAVE_FILE);
            io.save(party);
            _message = Strings::SaveComplete;
            return true;
        }
        default:
            return false;
    }
}

bool MenuManager::processMemberCommand(void)
{
    switch(MainMenuItem(_selectedMenuIndex))
    {
        case MainMenuItem::MagicSelected:
            _selectedSpellIndex = 0;
            state(MenuState::SelectSpell);
            return true;
        case MainMenuItem::PartySelected:
            state(MenuState::ReorderMember);
            _selectedPositionIndex = _selectedMemberIndex;
            return true;
        default:
            return false;
    }
}

bool MenuManager::processReorderMemberCommand(Party& party)
{
    party.reorder(_selectedMemberIndex, _selectedPositionIndex);
    _selectedMemberIndex = _selectedPositionIndex;
    state(MenuState::SelectMember);
    return true;
}

bool MenuManager::processComponentCommand(void)
{
    _selectedRuneIndex = 0;
    state(MenuState::SelectRune);
    return true;
}

bool MenuManager::processSpellCommand(Party& party)
{
    if (_selectedComponentIndex < -1)
        return cast(party, (Mob&)*party.memberAt(_selectedMemberIndex), _selectedSpellIndex);

    _selectedComponentIndex = 0;

    // Go straight to the rune level if there are no runes already in the spell.
    std::vector<Command*> spells = party.memberAt(_selectedMemberIndex)->spells();
    if (int(spells.size()) <= _selectedSpellIndex || spells.at(_selectedSpellIndex)->components().size() <= 0)
    {
        _selectedRuneIndex = 0;
        state(MenuState::SelectRune);
    }
    else
        state(MenuState::SelectComponent);

    return true;
}

/**
 * Adds a rune to the selected PC's selected Spell.
 */
bool MenuManager::processRuneCommand(const Party& party)
{
    PC* pc = party.memberAt(_selectedMemberIndex);
    if (int(pc->spells().size()) <= _selectedSpellIndex)
        pc->addSpell(new Spell(std::vector<Rune*>(0)));

    Spell* workingSpell = (Spell*) pc->spells().at(_selectedSpellIndex);

    if (_selectedRuneIndex == 0)
    {
        workingSpell->removeComponent(_selectedComponentIndex);
        return true;
    }

    workingSpell->component(_selectedComponentIndex, party.runeCollection().at(_selectedRuneIndex - 1));
    if (_selectedComponentIndex + 1 < int(pc->runeSlots()))
        _selectedComponentIndex++;
    return true;
}

bool MenuManager::cast(Play::Party& party, Play::Mob& caster, int spellIndex)
{
    using namespace Magic;
    Spell* spell = (Spell*) caster.spells().at(spellIndex);

    SpellContext context = SpellContext(party.members(), std::vector<Enemy*>());

    spell->execute(&caster, context);
    return true;
}


Play::PlayState MenuManager::start(void)
{
    return Play::PlayState::Exit;
}
