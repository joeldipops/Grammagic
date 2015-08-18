#ifndef MAPOBJECT_H_INCLUDED
#define MAPOBJECT_H_INCLUDED

#include <string>

#include "globalConstants.h"
#include "util/utils.h"

using namespace Util;

namespace Play
{
    class Party;
    struct PlayStateContainer;
    class MapObject
    {
        public:
            MapObject(void);
            virtual ~MapObject(void) {};
            bool isDense(void);
            std::string imageFileName(std::string);
            std::string imageFileName(void) const;

            virtual PlayStateContainer& onInspect(PlayStateContainer&) = 0;

            Location location(int, int);
            Location location(const Location*);

            Direction facing(void) const;
            Direction facing(Direction);

            virtual int x(int);
            virtual int x(void) const;
            virtual int y(int);
            virtual int y(void) const;
            virtual bool isMob(void) const { return false; };
            virtual bool isPlayerParty(void) const {return false;};

        protected:
            bool isDense(bool);
        private:
            bool _isDense;
            std::string _imageFileName;
            int _x;
            int _y;
            Direction _facing = Direction::NONE;
    };
}

#endif
