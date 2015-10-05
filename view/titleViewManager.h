#ifndef INTROVIEWMANAGER_H_INCLUDED
#define INTROVIEWEMANAGER_H_INCLUDED

#include "viewManager.h"

namespace View
{
    class TitleViewManager : public ViewManager
    {
        public:
            TitleViewManager(void);
            TitleViewManager(SDL_Renderer*, SDL_Rect, AssetCache*);
            void render(const std::vector<MenuItem*>, const int);
    };
}

#endif
