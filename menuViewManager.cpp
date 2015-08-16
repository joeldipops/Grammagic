#include "menuViewManager.h"

using namespace Play;

const SDL_Rect MenuViewManager::_menuControl = SDL_Rect { 6, 6, 175, 50 };
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
void MenuViewManager::renderRunes(const PC& pc, int runeIndex)
{
    std::vector<Rune> runes = std::vector<Rune>();
    for (Word* word : Templates::Commands::allCommands)
    {
        Rune item = Rune(word);
        runes.push_back(item);
    }

    std::vector<Rune*> pointers = toPointers(runes);
    std::vector<MenuItem*> items = std::vector<MenuItem*> (0);
    MenuItem item = MenuItem("");
    items.push_back(&item);
    items.insert(items.end(), pointers.begin(), pointers.end());
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
    SDL_Texture* cursor = assets()->get(RESOURCE_LOCATION + "cursor.png");
    SDL_Texture* valid = assets()->get(RESOURCE_LOCATION + "valid.png");
    SDL_Texture* invalid = assets()->get(RESOURCE_LOCATION + "invalid.png");

    SDL_Rect rect = SDL_Rect { _spellsVp.x + marginLeft, _spellsVp.y, WIDTH, _control.y + _control.h };

    for (int i = 0; i < pc.spellSlots(); i++)
    {
        int position = -1;
        if (i == spellIndex)
        {
            SDL_Rect cursorRect = SDL_Rect {_spellsVp.x + cursorXOffset, _spellsVp.y + cursorYOffset + rect.y, 25, 25};
            SDL_RenderCopy(renderer(), cursor, 0, &cursorRect);
            position = componentPosition;
        }
        std::vector<MenuItem*> pointers = std::vector<MenuItem*>(0);
        pointers.reserve(pc.runeSlots());

        if (i < int(pc.spells()->size()))
        {
            const Command* command = &pc.spells()->at(i);
            std::vector<Word*> words = command->components();

            std::vector<Rune> runes = std::vector<Rune>(0);
            pointers.reserve(words.size());
            runes.reserve(words.size());

            int maxSlots = pc.runeSlots() > int(words.size())
            ? words.size() + 1
            : pc.runeSlots();

            MenuItem emptySlot = MenuItem();

            for (int j = 0; j < maxSlots; j++)
            {
                if (j < int(words.size()))
                {
                    runes.push_back(Rune(words.at(j)));
                    pointers.push_back((MenuItem*)&runes.at(runes.size()-1));
                }
                else
                    pointers.push_back(&emptySlot);
            }

            SDL_Rect validRect = SDL_Rect { rect.w - 40, rect.y + cursorYOffset, 30, 30 };
            if (command->spell()->isValid(true))
                SDL_RenderCopy(renderer(), valid, 0, &validRect);
            else
                SDL_RenderCopy(renderer(), invalid, 0, &validRect);

            drawHorizontalControls(&pointers, position, &rect, &_menuControl);
        }
        else
        {
            pointers.push_back(new MenuItem());
            drawHorizontalControls(&pointers, position, &rect, &_menuControl);

            for (MenuItem* item : pointers)
                delete item;
        }

        rect = SDL_Rect { rect.x, rect.y + rect.h, rect.w, rect.h};
    }
}

void MenuViewManager::renderPCs(const Party& party, int memberIndex, int selectedPosition)
{
    bool isOrdering = selectedPosition >= 0 && memberIndex >= 0;
    SDL_Rect templateRect{ _partyVp.x + cursorXOffset, _partyVp.y, _partyVp.w - cursorXOffset, 150 };
    for (unsigned int i = 0; i < party.members().size(); i++)
    {
        unsigned int position = i;
        unsigned int uMemberIndex = memberIndex;
        unsigned int uSelectedPosition = selectedPosition;
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
            colour = &selectedColour;
        else
            colour = &textColour;

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
    fillViewport(&hudColour);
    drawBorder(borderWidth, &textColour);

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
                    drawBorder(_spellsVp, borderWidth, &textColour, true);
                    drawBorder(_runesVp, borderWidth, &textColour, true);
                    renderSpells(*party.memberAt(model.SelectedPCIndex), model.SelectedSpellIndex, model.SelectedComponentIndex);
                    if (model.SelectedRuneIndex >= 0)
                        renderRunes(*party.memberAt(model.SelectedPCIndex), model.SelectedRuneIndex);
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
