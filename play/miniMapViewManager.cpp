#include "viewManagers.h"

MiniMapViewManager::MiniMapViewManager(SDL_Renderer* r, SDL_Rect v, AssetCache* a) : ViewManager(r, v, a) {}

void MiniMapViewManager::render(void)
{
    ViewManager::render();
    fillViewport(&hudColour);
    drawBorder(DEFAULT_BORDER_WIDTH, &borderColour);
}



