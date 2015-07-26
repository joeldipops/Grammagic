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
void MapViewManager::render(const GameMap& gameMap, const Play::PlayState state)
{
    // Render Map
    ViewManager::render();
    renderTerrain(gameMap);
    renderContents(gameMap);
}

/**
 * Renders each terrain in the map.
 * @param mapData The map containing terrain data.
 */
void MapViewManager::renderTerrain(const GameMap& gameMap)
{
    for(int y = 0; y < gameMap.height(); y++)
    {
        for(int x = 0; x < gameMap.width(); x++)
        {
            const MapCell* cell = gameMap.getCell(x, y);
            std::string fileName = cell->terrain()->imageFileName();
            SDL_Texture* image = assets()->get(fileName);

            // render terrain
            SDL_Rect rect = { x * CELL_WIDTH, y * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT };
            SDL_RenderCopy(renderer(), image, NULL, &rect);
        }
    }
}

/**
 * Renders each mob in the map.
 * @param mobs The list of mobs to render.
 */
void MapViewManager::renderContents(const GameMap& gameMap)
{
    const std::vector<MapObject*> contents = gameMap.contents();
    for (unsigned int i = 0; i < contents.size(); i++)
    {
        SDL_Rect rect = {
            contents.at(i)->x() * CELL_WIDTH + (CELL_WIDTH / 4),
            contents.at(i)->y() * CELL_HEIGHT + (CELL_HEIGHT / 4),
            CELL_WIDTH / 2, CELL_HEIGHT / 2
        };

        SDL_Texture* image = assets()->get(contents.at(i)->imageFileName());
        SDL_RenderCopy(renderer(), image, NULL, &rect);

        if (contents.at(i)->isMob())
            renderHealthBar(*(Mob*)contents.at(i));
    }
}

void MapViewManager::renderHealthBar(const Mob& mob)
{
    const int barWidth = 2;
    SDL_Rect rect = {
        mob.x() * CELL_WIDTH + 1,
        mob.y() * CELL_HEIGHT + 2,
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


