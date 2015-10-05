#ifndef VICTORYVIEWMANAGER_H_INLCUDED
#define VICTORYVIEWMANAGER_H_INCLUDED

#include "viewManager.h"

namespace Play { class Party; }

namespace View
{
    class VictoryViewManager : public ViewManager
    {
        public:
            VictoryViewManager(SDL_Renderer*, SDL_Rect, AssetCache*);
            void render(Play::Party&);
    };
}

#endif
