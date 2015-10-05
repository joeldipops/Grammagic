#ifndef STATSVIEWMANAGER_H_INCLUDED
#define STATSVIEWMANAGER_H_INCLUDED

#include "viewManager.h"

namespace Play {class GameMap; }

namespace View
{
    using namespace Play;
    class StatsViewManager : public ViewManager
    {
        public:
            StatsViewManager(SDL_Renderer*, SDL_Rect, AssetCache*);
            void render(const GameMap&, const PlayState, int = -1);
    };
}

#endif
