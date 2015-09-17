#include "menuItem.h"

//{ Lifecycle
/**
 * Constructor
 */
MenuItem::MenuItem(const std::string& name_, const std::string& imagePath_)
{
    _name = name_;
    _imagePath = imagePath_;
}
//}


//{ Properties

/**
 * Gets the display name of this command.
 */
const std::string& MenuItem::name(void) const { return _name; }

/**
 * Gets the file path of an image to represent this command.
 */
const std::string& MenuItem::imagePath(void) const { return _imagePath; }

/**
 * @return The display text colour of this item.
 */
SDL_Colour MenuItem::colour(void) const
{
    return SDL_Colour { 0xFF, 0xFF, 0xFF, 0xFF };
}

//}

//{ Methods

const bool MenuItem::equals(MenuItem other) const
{
    return _name == other.name();
}

///}
