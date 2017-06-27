#include "victoryViewManager.h"

#include "../res/strings.h"
#include "../play/party.h"

using namespace View;

//{ Lifecycle
VictoryViewManager::VictoryViewManager(SDL_Renderer* r, SDL_Rect v, AssetCache* a)
: ViewManagerMenuToolsBase(r, v, a)
{
}
//}

//{ Methods
void VictoryViewManager::render(Party& party)
{
    using namespace Resources;
    const SDL_Rect controlView = {0, 0, 1200, 150};
    const SDL_Rect letterSize { 0, 0, 50, 50};
    ViewManager::render();
    fillViewport(BG_COLOUR);
    drawMessage(Strings::Victory, letterSize, controlView, false);

    SDL_Rect vp { viewPort().x, viewPort().y + 50, viewPort().w, viewPort().h - 50 };
    renderPCs(party, -1, vp, -1, true);
}
//}
