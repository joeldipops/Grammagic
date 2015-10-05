#ifndef MINIMAPVIEWMANAGER_H_INCLUDED
#define MINIMAPVIEWMANAGER_H_INCLUDED

#include "viewManager.h"

namespace Util { class AssetCache; }

namespace View
{
    class MiniMapViewManager : public ViewManager
    {
        public:
            MiniMapViewManager(SDL_Renderer*, SDL_Rect, Util::AssetCache*);
            void render(void);
    };
}
#endif
