#ifndef VIEWMANAGER_H_INCLUDED
#define VIEWMANAGER_H_INCLUDED

#include <algorithm>
#include <vector>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

#include "../util/utils.h"
#include "../globalConstants.h"

namespace Util { class AssetCache; }
namespace Core { class MenuItem; }
namespace Play { class Party; }

namespace View
{
    class ViewManager
    {
        public:
            static const SDL_Colour BG_COLOUR;
            static const SDL_Colour TEXT_COLOUR;
            static const SDL_Colour SELECTED_COLOUR;
            static const SDL_Colour HIGHLIGHTED_COLOUR;
            static const SDL_Colour INVISIBLE;

            static const SDL_Rect messageBoxOuter;
            static const SDL_Rect letterSize;
            static const int DEFAULT_BORDER_WIDTH;

            static const SDL_Rect _control;
            static const int controlBorderWidth;

            ViewManager(void) {};
            ViewManager(SDL_Renderer*, SDL_Rect&, Util::AssetCache*);
            virtual ~ViewManager(void);
            virtual void render(void);

            virtual int menuItemsPerColumn(void) const;

        protected:
            void fillViewport(const SDL_Colour&);
            void drawBorder(const int, const SDL_Colour*);
            void drawBorder(const SDL_Rect&, const int, const SDL_Colour*, const bool);

            void drawOptionBox(const SDL_Rect&, const std::string&, const int, const SDL_Colour&, const SDL_Colour&, const SDL_Colour&);
            virtual void drawOptionBox(const SDL_Rect&, const Core::MenuItem*, int borderWidth, const SDL_Colour&, const SDL_Colour&, const SDL_Colour&);
            void drawOptionBox(const SDL_Rect&, SDL_Texture*, int borderWidth, const SDL_Colour&, const SDL_Colour&, const SDL_Colour&);

            void drawControls(
                const std::vector<const Core::MenuItem*>&, const int, const SDL_Rect* = nullptr,
                const SDL_Rect* = nullptr, bool = true
            );
            void drawControls(
                const std::vector<Core::MenuItem*>&, const int, const SDL_Rect* = nullptr,
                const SDL_Rect* = nullptr, bool = true
            );
            void drawHorizontalControls(
                const std::vector<Core::MenuItem*>&, const int,
                const SDL_Rect* = nullptr, const SDL_Rect* = nullptr,
                const SDL_Colour* = nullptr, const SDL_Colour* = nullptr
            );
            SDL_Renderer* renderer(void);
            SDL_Rect viewPort(void) const;
            Util::AssetCache* assets(void);
            TTF_Font* font(void);
            SDL_Texture* formatFontTexture(const std::string, const SDL_Colour*);
            std::string displayMultiplier(float) const;


            void drawSector(int cx, int cy, int r, int degStart, int degEnd);
            natural drawMessage(const std::string&, const SDL_Rect&, const SDL_Rect&, bool shrinkToFit = false);

        private:
            void addToQuad(std::vector<std::vector<Util::Location>>&,int, int, int, int);
            SDL_Renderer* _renderer;
            SDL_Rect _viewPort;
            Util::AssetCache* _assets;
            int _menuItemsPerColumn;
    };
}
#endif
