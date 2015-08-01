#ifndef MAPOBJECT_H_INCLUDED
#define MAPOBJECT_H_INCLUDED

#include <string>

#include "util/utils.h"

using namespace Util;

class MapObject
{
    public:
        MapObject(void);
        virtual ~MapObject(void) {};
        bool isDense(void);
        std::string imageFileName(std::string);
        const std::string imageFileName(void) const;

        Location location(int, int);
        Location location(const Location*);
        virtual int x(int);
        virtual int x(void) const;
        virtual int y(int);
        virtual int y(void) const;
        virtual bool isMob(void) const { return false; };
        virtual bool isPlayerParty(void) const {return false;};


        static const std::string RESOURCE_LOCATION;

    protected:
        bool isDense(bool);
    private:
        bool _isDense;
        std::string _imageFileName;
        int _x;
        int _y;
};

#endif
