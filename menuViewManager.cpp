#include "menuViewManager.h"
#include "magic/battleCommands.h"

MenuViewManager::MenuViewManager(SDL_Renderer* r, SDL_Rect v, AssetCache* a)
    : ViewManager(r, v, a)
{
    _spellsVp = SDL_Rect {v.x, v.y, 0, 300};
    _runesVp = SDL_Rect {v.x, v.y + _spellsVp.h, 0, v.h - _spellsVp.h};

}

void MenuViewManager::render(Mob* pc)
{
    ViewManager::render();
    fillViewport(&hudColour);
    drawBorder(5, &borderColour);

    // Render Spells
    for (Command command : pc->commands())
    {
        ;
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
