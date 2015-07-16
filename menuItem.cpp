#include "menuItem.h"

MenuItem::MenuItem(void) {}
MenuItem::MenuItem(std::string name_)
{
    _name = name_;
}

/**
 * Gets or sets the display name of this command.
 */
    std::string MenuItem::name(void) const
{
    return _name;
}
std::string MenuItem::name(std::string name_)
{
    _name = name_;
    return _name;
}

const bool MenuItem::equals(MenuItem other) const
{
    return _name == other.name();
}
