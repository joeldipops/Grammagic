#include "menuViewManager.h"
#include "../res/strings.h"
#include "../magic/spell.h"
#include "../util/assetCache.h"

using namespace View;

const SDL_Rect MenuViewManager::_runeControl = SDL_Rect { 6, 6, 60, 60 };
/**
 * Constructor.
 */
MenuViewManager::MenuViewManager(SDL_Renderer* r, SDL_Rect v, AssetCache* a)
    : ViewManagerMenuToolsBase(r, v, a)
{
    _mainVp = SDL_Rect {v.x, v.y, 190, v.h };
    _spellsVp = SDL_Rect {v.x + _mainVp.w, v.y, v.w - _mainVp.w, 550};
    _runesVp = SDL_Rect {_spellsVp.x, v.y + _spellsVp.h, _spellsVp.w, v.h - _spellsVp.h };
    _partyVp = SDL_Rect { v.x + _mainVp.w, v.y, v.w - _mainVp.w, v.h };
}

/**
 * Renders the selectable list of runes
 * @param pc
 * @param runeIndex The rune that the cursor is on.
 */
void MenuViewManager::renderRunes(const Party& party, int runeIndex)
{
    std::vector<Rune*> runes = party.runeCollection();
    std::vector<MenuItem*> items = std::vector<MenuItem*> (0);
    MenuItem item = MenuItem("");
    items.push_back(&item);
    items.insert(items.end(), runes.begin(), runes.end());
    drawControls(items, runeIndex, &_runesVp, &_runeControl);
}

/**
 * Renders the PC's list of constructed spells.
 * @param pc the PC
 * @param spellIndex Index of the spell currently being edited.
 * @param componnentPosition Position of the component that can be changed.
 */
void MenuViewManager::renderSpells(const PC& pc, int spellIndex, int componentPosition)
{
    using namespace Resources;
    SDL_Texture* valid = assets()->get(RESOURCE_LOCATION + "valid.png");
    SDL_Texture* invalid = assets()->get(RESOURCE_LOCATION + "invalid.png");

    SDL_Rect rect = SDL_Rect { _spellsVp.x + marginLeft, _spellsVp.y, WIDTH, _control.y + _control.h };

    for (int i = 0; i < int(pc.spellSlots()); i++)
    {
        int position = -1;
        if (i == spellIndex)
        {
            position = componentPosition;
        }
        std::vector<MenuItem*> menuItems = std::vector<MenuItem*>(0);

        if (i < int(pc.spells().size()))
        {
            const Spell* command = (Spell*) pc.spells().at(i);
            std::vector<Rune*> runes = command->components();

            MenuItem emptySlot = MenuItem();

            menuItems.insert(menuItems.end(), runes.begin(), runes.end());

            if (runes.size() < pc.runeSlots())
                menuItems.push_back(&emptySlot);

            SDL_Rect validRect = SDL_Rect { rect.x - 30, rect.y + cursorYOffset, 30, 30 };
            if (command->isValid(true))
                SDL_RenderCopy(renderer(), valid, 0, &validRect);
            else
                SDL_RenderCopy(renderer(), invalid, 0, &validRect);

            // if position is less than -1 a different button (eg. "Cast") is selected.
            if (i == spellIndex && position >= -1)
                drawHorizontalControls(menuItems, position, &rect, &_runeControl, &SELECTED_COLOUR, &HIGHLIGHTED_COLOUR);
            else
                drawHorizontalControls(menuItems, position, &rect, &_runeControl);

            // Draw button to let us use the spell.
            if (command->isValid(true) && i == spellIndex)
            {
                SDL_Rect castRect { rect.w - (_menuControl.w + 10), rect.y + 10, _menuControl.w, _menuControl.h };
                if (position == -2)
                    drawOptionBox(castRect, Strings::Cast, borderWidth, BG_COLOUR, SELECTED_COLOUR, SELECTED_COLOUR);
                else
                    drawOptionBox(castRect, Strings::Cast, borderWidth, BG_COLOUR, TEXT_COLOUR, TEXT_COLOUR);
            }
        }
        else
        {
            MenuItem emptySlot = MenuItem();
            menuItems.push_back(&emptySlot);
            if (i == spellIndex)
                drawHorizontalControls(menuItems, position, &rect, &_menuControl, &SELECTED_COLOUR, &HIGHLIGHTED_COLOUR);
            else
                drawHorizontalControls(menuItems, position, &rect, &_menuControl);
        }

        rect = SDL_Rect { rect.x, rect.y + rect.h, rect.w, rect.h};
    }
}


/**
 * Renders menu related  elements to the screen.
 */
void MenuViewManager::render(const Party& party, const MenuViewModel& model, const std::string* message)
{
    ViewManager::render();
    fillViewport(BG_COLOUR);
    drawBorder(borderWidth, &TEXT_COLOUR);

    auto pointers = toPointers(model.MenuItems);

    // Highlight the drilled-into item.
    if (model.state != MenuState::SelectMenu)
    {
        auto copy = *pointers.at(model.SelectedMenuItem);
        copy.colour(HIGHLIGHTED_COLOUR);
        pointers.at(model.SelectedMenuItem) = &copy;
    }

    drawControls(pointers, int(model.SelectedMenuItem), &_mainVp, &_menuControl, false);

    switch(model.SelectedMenuItem)
    {
        case MagicSelected:
        case PartySelected:
            switch (model.state)
            {
                case MenuState::SelectMenu:
                case MenuState::SelectMember:
                    renderPCs(party, model.SelectedPCIndex, _partyVp);
                    break;
                case MenuState::ReorderMember:
                    renderPCs(party, model.SelectedPCIndex, _partyVp, model.SelectedPositionIndex);
                    break;
                default: {
                    drawBorder(_spellsVp, borderWidth, &TEXT_COLOUR, true);
                    drawBorder(_runesVp, borderWidth, &TEXT_COLOUR, true);
                    renderSpells(*party.memberAt(model.SelectedPCIndex), model.SelectedSpellIndex, model.SelectedComponentIndex);
                    renderRunes(party, model.SelectedRuneIndex);
                }
            }
            break;
        case SaveSelected:
        default:
             break;
    }

    if (message != nullptr)
    {
        drawMessage(*message, letterSize, messageBoxOuter, true);
    }

    SDL_RenderPresent(renderer());
}
