#include "miniMapViewManager.h"
using namespace View;

MiniMapViewManager::MiniMapViewManager(SDL_Renderer* r, SDL_Rect v, Util::AssetCache* a) : ViewManager(r, v, a) {}

void MiniMapViewManager::render(void)
{
    ViewManager::render();
    fillViewport(BG_COLOUR);
    drawBorder(DEFAULT_BORDER_WIDTH, &TEXT_COLOUR);
}



