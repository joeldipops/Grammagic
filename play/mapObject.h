#ifndef MAPOBJECT_H_INCLUDED
#define MAPOBJECT_H_INCLUDED

#include <string>

#include "../globalConstants.h"
#include "../util/utils.h"
#include "../res/templates.h"
#include "../util/events.h"

using namespace Util;

namespace Play
{
    class Party;
    struct PlayStateContainer;
    struct MapObjectTemplate;
    typedef PlayStateContainer& (*PlayEventHandler)(MapObject*, PlayStateContainer&);
    class MapObject
    {
        public:
            MapObject(const Resources::MapObjectTemplate&);
            virtual ~MapObject(void) {};
            bool isDense(void);
            std::string imageFileName(const std::string&);
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
            const Handler<MapObject, PlayStateContainer> onInspectFn(void) const;
        private:
            bool _isDense;
            std::string _imageFileName;
            int _x = 0;
            int _y = 0;
            Direction _facing = Direction::NONE;
            Handler<MapObject, PlayStateContainer> _onInspect;
    };
}

#endif
