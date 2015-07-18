#ifndef MENUITEM_H_INCLUDED
#define MENUITEM_H_INCLUDED

#include <string>
#include <SDL2/SDL.h>

class MenuItem
{
    public:
        MenuItem(void);
        MenuItem(std::string name);
        virtual ~MenuItem(void) {};

        virtual std::string name(void) const;
        virtual std::string name(std::string);
        virtual SDL_Colour colour(void) const;
        const bool equals(MenuItem) const;
    private:
        std::string _name;
};

#endif
