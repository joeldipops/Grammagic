#include "menuViewManager.h"

#include <iostream>

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
    _mainVp = SDL_Rect {v.x, v.y, 190, HEIGHT };
    _spellsVp = SDL_Rect {v.x + _mainVp.w, v.y, WIDTH - _mainVp.w, 550};
    _runesVp = SDL_Rect {_spellsVp.x, v.y + _spellsVp.h, _spellsVp.w, v.h - _spellsVp.h };
}

/**
 * Renders the selectable list of runes
 * @param pc
 * @param runeIndex The rune that the cursor is on.
 */
void MenuViewManager::renderRunes(const PC& pc, int runeIndex)
{
    std::vector<Rune> runes = std::vector<Rune>();
    for (Word* word : Commands::allCommands)
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
    SDL_Texture* cursor = assets()->get("res/cursor.png");
    SDL_Texture* valid = assets()->get("res/valid.png");
    SDL_Texture* invalid = assets()->get("res/invalid.png");

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


void MenuViewManager::render(const Party& party, const MenuViewModel& model, std::string* message)
{
    ViewManager::render();
    fillViewport(&hudColour);
    drawBorder(borderWidth, &borderColour);
    drawBorder(_spellsVp, borderWidth, &borderColour, true);
    drawBorder(_runesVp, borderWidth, &borderColour, true);

    auto pointers = toPointers(model.MenuItems);
    drawControls(&pointers, int(model.SelectedMenuItem), &_mainVp, &_menuControl, false);

    switch(model.SelectedMenuItem)
    {
        case MagicSelected:
            renderSpells(*party.leader(), model.SelectedSpellIndex, model.SelectedComponentIndex);
            if (model.SelectedRuneIndex >= 0)
                renderRunes(*party.leader(), model.SelectedRuneIndex);
            break;
        case SaveSelected:
        default:
            break;
    }

    if (message != nullptr)
    {
        drawMessage(*message, messageBoxOuter, messageBoxInner, DEFAULT_BORDER_WIDTH);
    }

    SDL_RenderPresent(renderer());
}
