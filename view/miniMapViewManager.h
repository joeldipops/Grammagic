#ifndef MINIMAPVIEWMANAGER_H_INCLUDED
#define MINIMAPVIEWMANAGER_H_INCLUDED

#include "viewManager.h"

namespace View
{
    class MiniMapViewManager : public ViewManager
    {
        public:
            MiniMapViewManager(SDL_Renderer*, SDL_Rect, AssetCache*);
            void render(void);
    };
}
#endif
