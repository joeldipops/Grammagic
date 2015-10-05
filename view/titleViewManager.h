#ifndef INTROVIEWMANAGER_H_INCLUDED
#define INTROVIEWEMANAGER_H_INCLUDED

#include "viewManager.h"

namespace View
{
    class TitleViewManager : public ViewManager
    {
        public:
            TitleViewManager(void);
            TitleViewManager(SDL_Renderer*, SDL_Rect, Util::AssetCache*);
            void render(const std::vector<Core::MenuItem*>, const int);
    };
}

#endif
