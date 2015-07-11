#ifndef VIEWMANAGER_H_INCLUDED
#define VIEWMANAGER_H_INCLUDED

#include <SDL2/SDL.h>
#include "util/assetCache.h"
#include <SDL2/SDL_ttf.h>
#include "menuItem.h"
#include <vector>

class ViewManager
{
    public:
        static const SDL_Colour hudColour;
        static const SDL_Colour borderColour;
        static const SDL_Colour textColour;
        static const int DEFAULT_BORDER_WIDTH;

        ViewManager(void) {};
        ViewManager(SDL_Renderer*, SDL_Rect, AssetCache*);
        virtual ~ViewManager(void);
        virtual void render(void);

        int menuItemsPerColumn(void) const;

    protected:
        void fillViewport(const SDL_Colour*);
        void drawBorder(const int, const SDL_Colour*);
        void drawBorder(const SDL_Rect, const int, const SDL_Colour*, const bool);
        void drawOptionBox(const SDL_Rect*, const std::string, const int, const SDL_Colour*, const SDL_Colour*, const SDL_Colour*);
        void drawControls(const std::vector<MenuItem*>*, const int, const SDL_Rect* = nullptr);
        SDL_Renderer* renderer(void);
        SDL_Rect viewPort(void);
        AssetCache* assets(void);
        TTF_Font* font(void);
        SDL_Texture* formatFontTexture(const std::string, const SDL_Colour*);

    private:
        SDL_Renderer* _renderer;
        SDL_Rect _viewPort;
        AssetCache* _assets;
        int _menuItemsPerColumn;

};
#endif
