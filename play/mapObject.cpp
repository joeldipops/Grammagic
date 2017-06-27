#include "mapObject.h"

using namespace Play;

//{ Lifecycle

MapObject::MapObject(const Resources::MapObjectTemplate& tmpl)
{
    _isDense = tmpl.IsDense;
    _imageFileName = tmpl.ImagePath;
    _onInspect = tmpl.OnInspect;
}

//}

//{ Properties

/**
 * Function trigged when object is inspected by player.
 */
const Handler<MapObject, PlayStateContainer> MapObject::onInspectFn(void) const { return _onInspect; }

/**
 * Sets and gets the path to this object's image.
 * @param name The filename.
 * @return the filename
 */
std::string MapObject::imageFileName(void) const { return _imageFileName; }
std::string MapObject::imageFileName(const std::string& name)
{
    _imageFileName = name;
    return _imageFileName;
}

/**
 * Gets or sets the Mob's X position.
 * @param x The horizontal co-ordinate of the map
 * @return
 */
int MapObject::x(void) const { return _x; }
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
int MapObject::y(void) const { return _y; }
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
bool MapObject::isDense(void) { return _isDense; }
bool MapObject::isDense(bool isDense_)
{
    _isDense = isDense_;
    return _isDense;
}

/**
 * Gets or sets the direction the mob is facing.
 */
Direction MapObject::facing(void) const { return _facing; }
Direction MapObject::facing(Direction facing_)
{
    _facing = facing_;
    return _facing;
}

//}

//{ Methods

/**
 * Gets or sets the X/Y coordinates of the mob.
 * @param x
 * @param y
 * @return
 */
Location MapObject::location(int x_, int y_)
{
    x(x_);
    y(y_);
    return Location(x_, y_);
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
        x(loc->X);
        y(loc->Y);
    }
    return Location(&_x, &_y);
}

//}
