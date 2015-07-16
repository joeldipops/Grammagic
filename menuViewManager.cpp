#include "menuViewManager.h"

#include <iostream>
#include "magic/battleCommands.h"

MenuViewManager::MenuViewManager(SDL_Renderer* r, SDL_Rect v, AssetCache* a)
    : ViewManager(r, v, a)
{
    _spellsVp = SDL_Rect {v.x, v.y, WIDTH, 550};
    _runesVp = SDL_Rect {v.x, v.y + _spellsVp.h, WIDTH, v.h - _spellsVp.h};
}

void MenuViewManager::render(PC* pc, int spellIndex, int componentPosition, int runeIndex)
{
    const int borderWidth = 5;
    const int cursorXOffset = borderWidth + controlMarginLeft;
    const int cursorYOffset = borderWidth + controlMarginTop;

    ViewManager::render();
    fillViewport(&hudColour);
    drawBorder(borderWidth, &borderColour);
    drawBorder(_spellsVp, borderWidth, &borderColour, true);
    drawBorder(_runesVp, borderWidth, &borderColour, true);

    SDL_Texture* cursor = assets()->get("res/cursor.png");
    SDL_Texture* valid = assets()->get("res/valid.png");
    SDL_Texture* invalid = assets()->get("res/invalid.png");

    // Render Spells
    SDL_Rect rect = SDL_Rect { marginLeft, 0, WIDTH, controlMarginTop + controlHeight };

    for (int i = 0; i < pc->spellSlots(); i++)
    {
        int position = -1;
        if (i == spellIndex)
        {
            SDL_Rect cursorRect = SDL_Rect {cursorXOffset, cursorYOffset + rect.y, 30, 30};
            SDL_RenderCopy(renderer(), cursor, 0, &cursorRect);
            position = componentPosition;
        }
        std::vector<MenuItem*> pointers = std::vector<MenuItem*>(0);
        pointers.reserve(pc->runeSlots());

        if (i < int(pc->spells()->size()))
        {
            Command command = pc->spells()->at(i);
            std::vector<Word*> words = command.components();

            std::vector<Rune> runes = std::vector<Rune>(0);
            pointers.reserve(words.size());
            runes.reserve(words.size());

            int deallocIndex = pc->runeSlots();

            int maxSlots = pc->runeSlots() > int(words.size())
            ? words.size() + 1
            : pc->runeSlots();

            for (int j = 0; j < maxSlots; j++)
            {
                if (j < int(words.size()))
                {
                    runes.push_back(Rune(words.at(j)));
                    pointers.push_back((MenuItem*)&runes.at(runes.size()-1));
                }
                else
                {
                    deallocIndex = j;
                    pointers.push_back(new MenuItem());
                }
            }

            SDL_Rect validRect = SDL_Rect { rect.w - rect.x, rect.y + cursorYOffset, 30, 30 };
            if (command.spell()->isValid(true))
                SDL_RenderCopy(renderer(), valid, 0, &validRect);
            else
                SDL_RenderCopy(renderer(), invalid, 0, &validRect);

            drawHorizontalControls(&pointers, position, &rect);

            for (int j = deallocIndex; j < int(pointers.size()); j++)
                delete pointers.at(j);
        }
        else
        {
            pointers.push_back(new MenuItem());
            drawHorizontalControls(&pointers, position, &rect);

            for (MenuItem* item : pointers)
                delete item;
        }

        rect = SDL_Rect { rect.x, rect.y + rect.h, rect.w, rect.h};
    }

    if (runeIndex >= 0)
    {
        // Render Runes
        std::vector<Rune> runes = std::vector<Rune>();
        for (Word* word : Commands::allCommands)
        {
            Rune item = Rune(word);
            runes.push_back(item);
        }

        std::vector<Rune*> pointers = toPointers(runes);
        std::vector<MenuItem*> items = std::vector<MenuItem*> {pointers.begin(), pointers.end()};
        drawControls(&items, runeIndex, &_runesVp);
    }

    SDL_RenderPresent(renderer());
    return;
}
