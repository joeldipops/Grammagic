#include "mapObject.h"

const std::string MapObject::RESOURCE_LOCATION = "res/";

MapObject::MapObject(void){}

/**
 * Sets and gets the path to this object's image.
 * @param name The filename.
 * @return the filename
 */
std::string MapObject::imageFileName(std::string name)
{
    _imageFileName = name;
    return _imageFileName;
}
const std::string MapObject::imageFileName(void) const
{
    return _imageFileName;
}

/**
 * Gets or sets the X/Y coordinates of the mob.
 * @param x
 * @param y
 * @return
 */
Location MapObject::location(int x, int y)
{
    _x = x;
    _y = y;
    return Location(&x, &y);
}

/**
 * Gets or sets the X/Y coordinates of the mob.
 * @param loc
 * @return
 */
Location MapObject::location(const Location* loc)
{
    if (loc != nullptr)
    {
        _x = loc->X;
        _y = loc->Y;
    }
    return Location(&_x, &_y);
}


/**
 * Gets or sets the Mob's X position.
 * @param x The horizontal co-ordinate of the map
 * @return
 */
int MapObject::x(void) const
{
    return _x;
}
int MapObject::x(int x_)
{
    _x = x_;
    return x();
}

/**
 * Gets or sets the Mob's Y position.
 * @param y The vertical co-ordinate on the map
 * @return
 */
int MapObject::y(void) const
{
    return _y;
}
int MapObject::y(int y_)
{
    _y = y_;
    return y();
}

/**
 * Sets and gets whether this object can be walked through.
 * @param The density.
 * @return The density.
 */
bool MapObject::isDense(void)
{
    return _isDense;
}
bool MapObject::isDense(bool isDense_)
{
    _isDense = isDense_;
    return _isDense;
}
