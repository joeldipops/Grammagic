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
        void render(const Mob*, Play::PlayState, const std::string& = "");
        natural lastDrawnCharCount(void) const;

    private:
        void writeHeading(void);
        void writeMessage(const std::string&);
        natural _lastDrawnCharCount;

};

class MapViewManager : public ViewManager
{
    public:
        MapViewManager(SDL_Renderer*, SDL_Rect, AssetCache*);
        void render(const GameMap*, const Play::PlayState);

    private:
        void renderHealthBar(const Mob&, int x, int y);
        void renderContents(const GameMap*, const SDL_Rect& visible);
        void renderTerrain(const GameMap* gameMap, const SDL_Rect& visible);
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
