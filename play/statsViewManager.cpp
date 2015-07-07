#include "viewManagers.h"

StatsViewManager::StatsViewManager(SDL_Renderer* r, SDL_Rect v, AssetCache* a) : ViewManager(r, v, a)
{
   _regions = std::vector<SDL_Rect>(0);
   _regions.push_back(SDL_Rect {1, 1, (viewPort().w / 2) - 5, 80});
}

void StatsViewManager::render(const GameMap* gameMap, const Play::PlayState state)
{
    ViewManager::render();
    fillViewport(&hudColour);
    drawBorder(DEFAULT_BORDER_WIDTH, &borderColour);

    const Mob* pc = gameMap->pc();
    SDL_Rect rect;

    for(const auto r : _regions)
    {
        // Render the character potrait
        SDL_RenderCopy(renderer(), assets()->get(pc->portraitFileName()), NULL, &r);
        std::string label = Strings::Stamina;
        std::string stamina = std::to_string(pc->stamina());
        rect = SDL_Rect{DEFAULT_BORDER_WIDTH + 2, r.h + r.y, viewPort().w - 75, 20};
        SDL_RenderCopy(renderer(), formatFontTexture(label, &textColour), NULL, &rect);
        rect = SDL_Rect{rect.x + rect.w + 15, rect.y, 40, rect.h};
        SDL_RenderCopy(renderer(), formatFontTexture(stamina, &textColour), NULL, &rect);
    }
}



