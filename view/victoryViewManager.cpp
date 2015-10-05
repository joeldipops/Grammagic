#include "victoryViewManager.h"
#include "../play/party.h"

using namespace View;

//{ Lifecycle
VictoryViewManager::VictoryViewManager(SDL_Renderer* r, SDL_Rect c, AssetCache* a)
: ViewManager(r, c, a) {}
//}

//{ Methods
void VictoryViewManager::render(Party& party)
{
    const SDL_Rect CONTROL_VIEW = {0, 0, 1000, 150};
    ViewManager::render();
    const SDL_Rect letterSize { 0, 0, 50, 50};
    drawMessage(Strings::Victory, letterSize, CONTROL_VIEW, false);
}
//}
