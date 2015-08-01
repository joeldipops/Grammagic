#ifndef GAMEMAP_H_INCLUDED
#define GAMEMAP_H_INCLUDED

#include <vector>

#include "mob.h"
#include "mapCell.h"
#include "../util/utils.h"
#include "pc.h"
#include "party.h"

namespace Play
{
    class MapCell;

    class GameMap
    {
        public:
            GameMap();
            GameMap(int, int);
            ~GameMap();

            void kill(MapObject*);
            void buryTheDead();

            bool place(MapObject*, int, int, bool = false);

            bool moveMob(MapObject*, Location);
            bool moveMob(MapObject*, int, int);
            MapCell* getCell(int x, int y);
            const MapCell* getCell(int x, int y) const;
            void setCell(int x, int y, MapCell*);

            int width(void) const;
            int height(void) const;
            std::vector<MapCell>* cells(std::vector<MapCell>* = nullptr);
            std::vector<MapObject*> contents(std::vector<MapObject*>);
            std::vector<MapObject*> contents(void) const;
            Party* party(Party&);
            Party* party(void) const;

        private:
            std::vector<MapObject*> _contents;
            std::vector<MapCell> _cells;
            int _width;
            int _height;
    };
}

#endif

