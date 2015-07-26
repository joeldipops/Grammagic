#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

#include <SDL2/SDL.h>
#include <string>

enum MobType
{
    // There is no mob here.
    None,
    // This mob is directly controlled by the player
    PlayerCharacter,
    // Some other NPC
    Hostile,

    PartyOfMobs
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
        Victory,
        GameOver,
        Exit
    };


    enum MenuState
    {
        SelectMenu,
        SelectSpell,
        SelectComponent,
        SelectRune,
    };

    enum MainMenuItem
    {
        MagicSelected,
        SaveSelected,
        SaveComplete,
    };
}

namespace Magic
{
    enum WordType
    {
        AVerb,
        AnAdjective,
        ANoun,
        AnAdverb,
        ANounPhrase
    };
}


const int WIDTH = 1200;
const int HEIGHT = 800;
const int BITS_PER_PIXEL = 32;
const std::string SAVE_FILE = "grammagic.save";


#endif
