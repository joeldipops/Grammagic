#ifndef VIEWMANAGERS_H_INCLUDED
#define VIEWMANAGERS_H_INCLUDED

#include <SDL2/SDL.h>
#include <algorithm>

#include "gameMap.h"
#include "../globalConstants.h"
#include "../viewManager.h"
#include "../res/strings.h"
#include "../util/utils.h"

class ControlViewManager : public ViewManager
{
    public:
        ControlViewManager(SDL_Renderer*, SDL_Rect, AssetCache*);
        void render(const Mob*, Play::PlayState);

    private:
        void writeHeading(void);
};

class MapViewManager : public ViewManager
{
    public:
        static const int CELL_WIDTH = 50;
        static const int CELL_HEIGHT = 50;
        MapViewManager(SDL_Renderer*, SDL_Rect, AssetCache*);
        void render(const GameMap&, const Play::PlayState);

    private:
        void renderHealthBar(const Mob&);
        void renderContents(const GameMap&);
        void renderTerrain(const GameMap&);
};

class MiniMapViewManager : public ViewManager
{
    public:
        MiniMapViewManager(SDL_Renderer*, SDL_Rect, AssetCache*);
        void render(void);
};

class StatsViewManager : public ViewManager
{
    public:
        StatsViewManager(SDL_Renderer*, SDL_Rect, AssetCache*);
        void render(const GameMap&, const Play::PlayState, int = -1);
};

#endif
