#ifndef GAMEMAP_H_INCLUDED
#define GAMEMAP_H_INCLUDED

#include <vector>
#include <deque>
#include <map>

#include "mob.h"
#include "mapCell.h"
#include "../util/utils.h"
#include "pc.h"
#include "party.h"
#include "../mapFileBlock.h"
#include "enemy.h"

namespace Play
{
    class MapCell;

    class GameMap
    {
        public:
            GameMap();
            GameMap(int, int);
            ~GameMap();

            Party* party(Party&);
            Party* party(void) const;
            const SDL_Rect visible(void) const;

            bool place(MapObject*, int, int, bool = false);
            void remove(MapObject* mob);
            void kill(MapObject*);
            void buryTheDead();
            bool requestNextChunk(void);
            bool loadChunk(int x, int y);
            bool loadChunk(int x, int y, const std::string& path);

            PlayStateContainer& onInspect(int x, int y, PlayStateContainer&);
            bool moveMob(MapObject*, Location);
            bool moveMob(MapObject*, int, int, PlayStateContainer* = nullptr);
            MapCell* getCell(int x, int y);
            const MapCell* getCell(int x, int y) const;
            void setCell(int x, int y, MapCell*);

            int width(void) const;
            int height(void) const;
            std::vector<MapObject*> contents(std::vector<MapObject*>);
            std::vector<MapObject*> contents(void) const;

        private:
            std::map<int, MapCell>* cells(std::map<int, MapCell>* = nullptr);
            void removeChunk(int x, int y);

            static const natural CHUNK_LIMIT = 5;
            static const natural CHUNK_WIDTH = 40;
            static const natural CHUNK_HEIGHT = 40;
            static const natural HORIZONTAL_VISION = 10;
            static const natural VERTICAL_VISION = 6;

            std::vector<MapObject*> _contents;
            std::map<unsigned long, MapCell> _cells;
            std::deque<SDL_Rect> _chunks;

            int _maxX = 0;
            int _maxY = 0;
    };
}

#endif

