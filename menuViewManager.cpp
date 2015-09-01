#include "menuViewManager.h"

using namespace Play;

const SDL_Rect MenuViewManager::_menuControl = SDL_Rect { 6, 6, 150, 50 };
const int MenuViewManager::cursorXOffset = borderWidth + _menuControl.x;
const int MenuViewManager::cursorYOffset = borderWidth + _menuControl.y;


/**
 * Constructor.
 */
MenuViewManager::MenuViewManager(SDL_Renderer* r, SDL_Rect v, AssetCache* a)
    : ViewManager(r, v, a)
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
    drawControls(&items, runeIndex, &_runesVp, &_menuControl);
}

/**
 * Renders the PC's list of constructed spells.
 * @param pc the PC
 * @param spellIndex Index of the spell currently being edited.
 * @param componnentPosition Position of the component that can be changed.
 */
void MenuViewManager::renderSpells(const PC& pc, int spellIndex, int componentPosition)
{
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

        if (i < int(pc.spells()->size()))
        {
            const Command* command = &pc.spells()->at(i);
            std::vector<Rune*> runes = command->components();

            MenuItem emptySlot = MenuItem();

            menuItems.insert(menuItems.end(), runes.begin(), runes.end());

            if (runes.size() < pc.runeSlots())
                menuItems.push_back(&emptySlot);

            SDL_Rect validRect = SDL_Rect { rect.w - 40, rect.y + cursorYOffset, 30, 30 };
            if (command->spell()->isValid(true))
                SDL_RenderCopy(renderer(), valid, 0, &validRect);
            else
                SDL_RenderCopy(renderer(), invalid, 0, &validRect);

            if (i == spellIndex)
                drawHorizontalControls(&menuItems, position, &rect, &_menuControl, &SELECTED_COLOUR, &HIGHLIGHTED_COLOUR);
            else
                drawHorizontalControls(&menuItems, position, &rect, &_menuControl);
        }
        else
        {
            MenuItem emptySlot = MenuItem();
            menuItems.push_back(&emptySlot);
            if (i == spellIndex)
                drawHorizontalControls(&menuItems, position, &rect, &_menuControl, &SELECTED_COLOUR, &HIGHLIGHTED_COLOUR);
            else
                drawHorizontalControls(&menuItems, position, &rect, &_menuControl);
        }

        rect = SDL_Rect { rect.x, rect.y + rect.h, rect.w, rect.h};
    }
}

void MenuViewManager::renderPCs(const Party& party, int memberIndex, int selectedPosition)
{
    bool isOrdering = selectedPosition >= 0 && memberIndex >= 0;
    SDL_Rect templateRect{ _partyVp.x + cursorXOffset, _partyVp.y, _partyVp.w - cursorXOffset, 150 };
    for (natural i = 0; i < party.members().size(); i++)
    {
        natural position = i;
        natural uMemberIndex = memberIndex;
        natural uSelectedPosition = selectedPosition;
        if (isOrdering)
        {
            if (uMemberIndex == i)
            {
                position = uSelectedPosition;
            }
            else
            {
                if (uSelectedPosition <= i && i <= uMemberIndex)
                    position = i + 1;
                else if (uMemberIndex <= i && i <= uSelectedPosition)
                    position = i - 1;
                else
                    position = i;
            }
        }

        SDL_Rect rect
        {
            templateRect.x + ((position == uSelectedPosition && isOrdering) ? cursorXOffset : 0),
            templateRect.y + (templateRect.h * int(position)),
            templateRect.w,
            templateRect.h
        };


        PC* pc = party.memberAt(i);
        const int TEXT_WIDTH = 22;
        const int TEXT_HEIGHT = 19;
        const int LABEL_GAP = 60;
        const int MARGIN_Y = 20;

        const SDL_Colour* colour;

        if (memberIndex == int(i))
            colour = &SELECTED_COLOUR;
        else
            colour = &TEXT_COLOUR;

        // Draw name;
        std::string name = pc->name();
        SDL_Rect nameRect {rect.x + 2, rect.y + 3, TEXT_WIDTH * int(name.length()), TEXT_HEIGHT };
        SDL_Texture* textTure = formatFontTexture(name, colour);
        SDL_RenderCopy(renderer(), textTure, NULL, &nameRect);

        // Draw Portrait
        SDL_Rect portRect { rect.x, nameRect.y + nameRect.h, 100,100 };
        SDL_RenderCopy(renderer(), assets()->get(pc->portraitFileName()), NULL, &portRect);

        // Draw class name
        std::string className = pc->className();
        SDL_Rect classRect { rect.x + 2, portRect.y + portRect.h, TEXT_WIDTH * int(className.length()), TEXT_HEIGHT};
        textTure = formatFontTexture(className, colour);
        SDL_RenderCopy(renderer(), textTure, NULL, &classRect);

        // Draw stamina.
        std::string label = "Stamina";
        SDL_Rect stmLabelRect { rect.x + TEXT_WIDTH * 10, rect.y + 3, TEXT_WIDTH * int(label.length()), TEXT_HEIGHT};

        SDL_RenderCopy(renderer(), formatFontTexture(label, colour), NULL, &stmLabelRect);

        // Draw skill.
        label = "Skill";
        std::string value = displayMultiplier(pc->defaultSkill());
        SDL_Rect sklLabelRect { stmLabelRect.x, stmLabelRect.y + stmLabelRect.h + MARGIN_Y, TEXT_WIDTH * int(label.length()), TEXT_HEIGHT};
        SDL_Rect sklValueRect { sklLabelRect.x + sklLabelRect.w + LABEL_GAP, stmLabelRect.y + stmLabelRect.h + MARGIN_Y, TEXT_WIDTH * int(value.length()), TEXT_HEIGHT };
        SDL_RenderCopy(renderer(), formatFontTexture(label, colour), NULL, &sklLabelRect);
        SDL_RenderCopy(renderer(), formatFontTexture(value, colour), NULL, &sklValueRect);

        // Draw resistance
        label = "Resist";
        value = displayMultiplier(pc->defaultResistance());
        SDL_Rect resLabelRect { sklLabelRect.x, sklLabelRect.y + sklLabelRect.h + MARGIN_Y, TEXT_WIDTH * int(label.length()), TEXT_HEIGHT};
        SDL_Rect resValueRect { sklValueRect.x, sklValueRect.y + sklValueRect.h + MARGIN_Y, TEXT_WIDTH * int(value.length()), TEXT_HEIGHT };
        SDL_RenderCopy(renderer(), formatFontTexture(label, colour), NULL, &resLabelRect);
        SDL_RenderCopy(renderer(), formatFontTexture(value, colour), NULL, &resValueRect);

        // Draw Speed
        label = "Speed";
        value = displayMultiplier(pc->defaultSpeed());
        SDL_Rect spdLabelRect { sklValueRect.x + sklValueRect.w + 2 * LABEL_GAP, sklValueRect.y, TEXT_WIDTH * int(label.length()), TEXT_HEIGHT};
        SDL_Rect spdValueRect { spdLabelRect.x + spdLabelRect.w + LABEL_GAP, sklValueRect.y, TEXT_WIDTH * int(value.length()), TEXT_HEIGHT };
        SDL_RenderCopy(renderer(), formatFontTexture(label, colour), NULL, &spdLabelRect);
        SDL_RenderCopy(renderer(), formatFontTexture(value, colour), NULL, &spdValueRect);

        // Draw Defense
        label = "Defense";
        value = displayMultiplier(pc->defaultDefence());
        SDL_Rect defLabelRect { spdLabelRect.x, spdLabelRect.y + spdLabelRect.h + MARGIN_Y, TEXT_WIDTH * int(label.length()), TEXT_HEIGHT};
        SDL_Rect defValueRect { spdValueRect.x, spdValueRect.y + spdValueRect.h + MARGIN_Y, TEXT_WIDTH * int(value.length()), TEXT_HEIGHT };
        SDL_RenderCopy(renderer(), formatFontTexture(label, colour), NULL, &defLabelRect);
        SDL_RenderCopy(renderer(), formatFontTexture(value, colour), NULL, &defValueRect);

        value = std::to_string(pc->stamina());
        SDL_Rect stmValueRect { sklValueRect.x, stmLabelRect.y, TEXT_WIDTH * int(value.length()), TEXT_HEIGHT };
        SDL_RenderCopy(renderer(), formatFontTexture(value, colour), NULL, &stmValueRect);

        drawBorder(rect, 3, colour, false);
    }
}


void MenuViewManager::render(const Party& party, const MenuViewModel& model, std::string* message)
{
    ViewManager::render();
    fillViewport(&BG_COLOUR);
    drawBorder(borderWidth, &TEXT_COLOUR);

    auto pointers = toPointers(model.MenuItems);
    drawControls(&pointers, int(model.SelectedMenuItem), &_mainVp, &_menuControl, false);

    switch(model.SelectedMenuItem)
    {
        case MagicSelected:
        case PartySelected:
            switch (model.state)
            {
                case MenuState::SelectMenu:
                case MenuState::SelectMember:
                    renderPCs(party, model.SelectedPCIndex);
                    break;
                case MenuState::ReorderMember:
                    renderPCs(party, model.SelectedPCIndex, model.SelectedPositionIndex);
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
