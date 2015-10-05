#ifndef MAPVIEWMANAGER_H_INCLUDED
#define MAPVIEWMANAGER_H_INCLUDED

#include "viewManager.h"

namespace Play { class GameMap; class Mob; }

namespace View
{
    using namespace Play;
    class MapViewManager : public ViewManager
    {
        public:
            MapViewManager(SDL_Renderer*, SDL_Rect, Util::AssetCache*);
            void render(const GameMap*, const Play::PlayState);

        private:
            void renderHealthBar(const Mob&, int x, int y);
            void renderContents(const GameMap*, const SDL_Rect& visible);
            void renderTerrain(const GameMap* gameMap, const SDL_Rect& visible);
    };
}

#endif
