#ifndef MENUITEM_H_INCLUDED
#define MENUITEM_H_INCLUDED

#include <string>
#include <SDL2/SDL.h>

namespace Core
{
    class MenuItem
    {
        public:
            MenuItem(const std::string& name = "", const std::string& imagePath = "");
            virtual ~MenuItem(void) {};

            virtual const std::string& name(void) const;
            virtual const std::string& imagePath(void) const;
            virtual SDL_Colour colour(void) const;
            virtual SDL_Colour colour(const SDL_Colour&);
            const bool equals(MenuItem) const;
        private:
            std::string _name = "";
            std::string _imagePath = "";
            SDL_Colour _colour;
    };
}
#endif
