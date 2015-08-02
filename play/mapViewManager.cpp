#include "viewManagers.h"

/**
 * Constructor passes through to base class.
 */
MapViewManager::MapViewManager(SDL_Renderer* r, SDL_Rect v, AssetCache* a) : ViewManager(r, v, a) {}

/**
 * Renders the map.
 * @param gameMap Renders the contents of this map to the window.
 * @param state Rendering may differ depending on the current state.
 */
void MapViewManager::render(const GameMap* gameMap, const Play::PlayState state)
{
    // Render Map
    ViewManager::render();


    SDL_Rect visible {
        gameMap->party()->x() - gameMap->width() / 2,
        gameMap->party()->y() - gameMap->height() / 2,
        gameMap->width(),
        gameMap->height()
    };

    renderTerrain(gameMap, visible);
    renderContents(*gameMap, visible);
}

/**
 * Renders each terrain in the map.
 * @param mapData The map containing terrain data.
 */
void MapViewManager::renderTerrain(const GameMap* gameMap, const SDL_Rect& visible)
{
    const std::string blank = "res/hidden.png";
    for(int y = visible.y; y < visible.h; y++)
    {
        for(int x = visible.x; x < visible.w; x++)
        {
            SDL_Rect rect = { (x - visible.x) * CELL_WIDTH, (y - visible.y) * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT };
            std::string fileName;

            if (x >= 0 && y >= 0)
            {
                const MapCell* cell = gameMap->getCell(x, y);
                if (cell != nullptr)
                    fileName = cell->terrain()->imageFileName();
                else
                    fileName = blank;
            }
            else
                fileName = blank;

            SDL_Texture* image = assets()->get(fileName);
            SDL_RenderCopy(renderer(), image, NULL, &rect);
        }
    }
}

/**
 * Renders each mob in the map.
 * @param mobs The list of mobs to render.
 */
void MapViewManager::renderContents(const GameMap& gameMap, const SDL_Rect& visible)
{
    const std::vector<MapObject*> contents = gameMap.contents();
    for (const MapObject* mob : gameMap.contents())
    {
        // Mob is not visible.
        if (mob->x() < visible.x || mob->x() > visible.x + visible.w)
            continue;
        if (mob->y() < visible.y || mob->y() > visible.y + visible.h)
            continue;

        SDL_Rect rect = {
            (mob->x() - visible.x) * CELL_WIDTH + (CELL_WIDTH / 4),
            (mob->y() - visible.y) * CELL_HEIGHT + (CELL_HEIGHT / 4),
            CELL_WIDTH / 2, CELL_HEIGHT / 2
        };

        SDL_Texture* image = assets()->get(mob->imageFileName());
        SDL_RenderCopy(renderer(), image, NULL, &rect);

        if (mob->isMob())
            renderHealthBar(*(Mob*)mob, mob->x() - visible.x, mob->y() - visible.y);
    }

    Mob& leader = *(Mob*)gameMap.party()->leader();
    renderHealthBar(leader, leader.x() - visible.x, leader.y() - visible.y);
}

void MapViewManager::renderHealthBar(const Mob& mob, int x, int y)
{
    const int barWidth = 2;
    SDL_Rect rect = {
        x * CELL_WIDTH + 1,
        y * CELL_HEIGHT + 2,
        CELL_WIDTH - 4,
        barWidth,
    };

    SDL_Colour colour = SDL_Colour {0x00, 0x00, 0x00, 0xFF};
    drawBorder(rect, 1, &colour , false);
    SDL_SetRenderDrawColor(renderer(), 0x00, 0xFF, 0x00, 0xFF);
    int w = ceil( (double(rect.w) / double(mob.maxStamina())) * mob.stamina() );

    rect = {
        rect.x,
        rect.y,
        w,
        rect.h
    };

    SDL_RenderFillRect(renderer(), &rect);
}


