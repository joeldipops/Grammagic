#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

#include <SDL2/SDL.h>

enum MobType
{
    // There is no mob here.
    None,
    // This mob is directly controlled by the player
    PC,
    // Some other NPC
    Hostile
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

namespace Play {
    enum PlayState
    {
        Combat,
        Movement,
        Victory,
        GameOver,
        Exit
    };
}


#endif
