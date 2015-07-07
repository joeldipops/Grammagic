#ifndef MENUITEM_H_INCLUDED
#define MENUITEM_H_INCLUDED

#include <string>

class MenuItem
{
    public:
        MenuItem(void);
        MenuItem(std::string name);
        const std::string name(void) const;
        std::string name(std::string);
        const bool equals(MenuItem) const;
    private:
        std::string _name;
};

#endif
