#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

#include <SDL2/SDL.h>
#include <string>
#include "core.h"

enum MobType
{
    // There is no mob here.
    None = '0',
    // This mob is directly controlled by the player
    PlayerCharacter = '2',
    // Some other NPC
    Hostile = '3',

    PartyOfMobs = '4',
    E1 = '5',
    E2 = '6',
    B1 = '7'
};

enum TerrainType
{
    WallTerrain = 'w',
    GrassTerrain = 'g',
    HutTerrain = 'h'
};

namespace Title
{
    enum TitleState
    {
        Normal,
        Exit
    };
}

namespace Core
{
    enum CoreState
    {
        Title,
        Play,
        Load,
        Exit
    };

    enum InputPress
    {
        UP = SDLK_w,
        DOWN = SDLK_s,
        LEFT = SDLK_a,
        RIGHT = SDLK_d
    };
}

namespace Play
{
    enum PlayState
    {
        Menu,
        Combat,
        Movement,
        Message,
        Victory,
        GameOver,
        Exit
    };


    enum MenuState
    {
        SelectMenu,
        SelectMember,
        ReorderMember,
        SelectSpell,
        SelectComponent,
        SelectRune,
    };

    enum MainMenuItem
    {
        MagicSelected,
        SaveSelected,
        PartySelected,
        SaveComplete
    };

    enum Direction
    {
        NONE,
        NORTH,
        SOUTH,
        EAST,
        WEST
    };
}

namespace Magic
{
    enum WordType
    {
        VERB,
        ADJECTIVE,
        NOUN,
        ADVERB,
        NOUN_PHRASE
    };
}

const int CELL_WIDTH = 50;
const int CELL_HEIGHT = 50;
const natural WIDTH = 1200;
const natural HEIGHT = 800;
const std::string RESOURCE_LOCATION = "res/images/";

const natural BITS_PER_PIXEL = 32;
const std::string SAVE_FILE = "grammagic.save";


#endif
