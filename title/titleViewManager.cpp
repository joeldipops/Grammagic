#include "titleStateManager.h"

TitleViewManager::TitleViewManager() : ViewManager() {}
TitleViewManager::TitleViewManager(SDL_Renderer* r, SDL_Rect v, AssetCache* a) : ViewManager(r, v, a) {}

void TitleViewManager::render(std::vector<MenuItem*> commands, int selectedIndex)
{
    ViewManager::render();
    SDL_SetRenderDrawColor(renderer(), 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer());

    drawBorder(5, &TEXT_COLOUR);
    drawControls(&commands, selectedIndex);
}
