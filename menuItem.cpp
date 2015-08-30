#include "menuItem.h"

MenuItem::MenuItem(void) {}
MenuItem::MenuItem(const std::string& name_)
{
    _name = name_;
}

/**
 * Gets or sets the display name of this command.
 */
const std::string& MenuItem::name(void) const { return _name; }

/**
 * @return The display text colour of this item.
 */
SDL_Colour MenuItem::colour(void) const
{
    return SDL_Colour { 0xFF, 0xFF, 0xFF, 0xFF };
}

const bool MenuItem::equals(MenuItem other) const
{
    return _name == other.name();
}
