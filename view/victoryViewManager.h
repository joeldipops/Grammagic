#ifndef VICTORYVIEWMANAGER_H_INLCUDED
#define VICTORYVIEWMANAGER_H_INCLUDED

#include "viewManagerMenuToolsBase.h"

namespace Util { class AssetCache; }
namespace Play { class Party; }

namespace View
{
    class VictoryViewManager : public ViewManagerMenuToolsBase
    {
        public:
            VictoryViewManager(SDL_Renderer*, SDL_Rect, Util::AssetCache*);
            void render(Play::Party&);
    };
}

#endif
