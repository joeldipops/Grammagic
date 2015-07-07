#ifndef GAMEMAP_H_INCLUDED
#define GAMEMAP_H_INCLUDED

#include "mob.h"
#include "mapCell.h"
#include <vector>
#include "../util/utils.h"

class MapCell;

class GameMap
{
    public:
        GameMap();
        GameMap(int, int);
        ~GameMap();

        void killMob(const Mob*);
        void buryTheDead();

        bool placeMob(Mob*, int, int);

        bool moveMob(Mob*, Location);
        bool moveMob(Mob*, int, int);
        MapCell* getCell(int x, int y);
        const MapCell* getCell(int x, int y) const;
        void setCell(int x, int y, MapCell*);

        int width(void) const;
        int height(void) const;
        std::vector<MapCell>* cells(std::vector<MapCell>* = nullptr);
        std::vector<Mob*> mobs(std::vector<Mob*>);
        std::vector<Mob*> mobs(void) const;
        Mob* pc(Mob* = nullptr);
        const Mob* pc(void) const;


    private:
        std::vector<Mob*> _mobs;
        std::vector<MapCell> _cells;
        int _width;
        int _height;
};

#endif

