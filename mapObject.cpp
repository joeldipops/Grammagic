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
