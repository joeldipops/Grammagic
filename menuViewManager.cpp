#include "menuViewManager.h"

#include <iostream>
#include "magic/battleCommands.h"

MenuViewManager::MenuViewManager(SDL_Renderer* r, SDL_Rect v, AssetCache* a)
    : ViewManager(r, v, a)
{
    _spellsVp = SDL_Rect {v.x, v.y, WIDTH, 300};
    _runesVp = SDL_Rect {v.x, v.y + _spellsVp.h, WIDTH, v.h - _spellsVp.h};

}

void MenuViewManager::render(Mob* pc)
{
    ViewManager::render();
    fillViewport(&hudColour);
    drawBorder(5, &borderColour);

    // Render Spells
    SDL_Rect rect = SDL_Rect { 0, 0, WIDTH, controlMarginTop + controlHeight };
    for (Command command : pc->commands())
    {
        std::vector<Word*> words = command.components();
        std::vector<MenuItem*> pointers = std::vector<MenuItem*>(0);
        std::vector<Rune> runes = std::vector<Rune>(0);
        pointers.reserve(words.size());
        runes.reserve(words.size());
        for (Word* word : words)
        {
            runes.push_back(Rune(word));
            pointers.push_back((MenuItem*)&runes.at(runes.size()-1));
        }
        if (!runes.size()) {
            continue;
        }

        drawHorizontalControls(&pointers, -1, &rect);
        rect = SDL_Rect { 0, rect.y + rect.h, rect.w, rect.h};
    }

    // Render Runes
    std::vector<Rune> runes = std::vector<Rune>();
    for (Word* word : Commands::allCommands)
    {
        Rune item = Rune(word);
        runes.push_back(item);
    }

    std::vector<Rune*> pointers = toPointers(runes);
    std::vector<MenuItem*> items = std::vector<MenuItem*> {pointers.begin(), pointers.end()};
    drawControls(&items, 0, &_runesVp);

    SDL_RenderPresent(renderer());
    return;
}
