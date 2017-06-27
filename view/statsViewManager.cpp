#include "statsViewManager.h"

#include "../res/strings.h"
#include "../play/gameMap.h"
#include "../util/assetCache.h"
using namespace View;

StatsViewManager::StatsViewManager(SDL_Renderer* r, SDL_Rect v, AssetCache* a) : ViewManager(r, v, a)
{}

void StatsViewManager::render(const GameMap& gameMap, const Play::PlayState state, int selectedMemberIndex)
{
    using namespace Resources;
    ViewManager::render();
    fillViewport(BG_COLOUR);
    drawBorder(DEFAULT_BORDER_WIDTH, &TEXT_COLOUR);

    SDL_Rect rect;
    SDL_Rect port = SDL_Rect {1, 1, (viewPort().w / 2) - 5, 80};

    for(natural i = 0; i < gameMap.party()->members().size(); i++)
    {
        const PC* pc = gameMap.party()->members().at(i);
        const SDL_Colour* colour = state == PlayState::Combat && selectedMemberIndex != int(i)
            ? &TEXT_COLOUR
            : &SELECTED_COLOUR
            ;

        // Render the character potrait
        SDL_RenderCopy(renderer(), assets()->get(pc->portraitFileName()), NULL, &port);
        int radius = port.h / 2;
        int x = port.x + port.w + radius;
        int y = port.y + radius;

        if (state == PlayState::Combat && pc->isBlocked())
        {
            int degree = 3.6 * pc->elapsedWait();
            if (degree > 0)
                drawSector(x, y, radius, 180, degree + 180);
        }

        std::string label = Strings::Stamina;
        std::string stamina = std::to_string(pc->stamina());
        rect = SDL_Rect{DEFAULT_BORDER_WIDTH + 2, port.h + port.y, viewPort().w - 75, 20};
        SDL_RenderCopy(renderer(), formatFontTexture(label, colour), NULL, &rect);
        rect = SDL_Rect{rect.x + rect.w + 15, rect.y, 40, rect.h};
        SDL_RenderCopy(renderer(), formatFontTexture(stamina, colour), NULL, &rect);

        port = SDL_Rect { port.x, rect.y + rect.h, port.w, port.h };
    }
}



