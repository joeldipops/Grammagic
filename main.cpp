#include <string>
#include "res/strings.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "util/assetCache.h"
#include "globalConstants.h"
#include "title/titleStateManager.h"
#include "play/playStateManager.h"

#include <stdlib.h>
#include <time.h>

#include "magic/battleCommands.h"

const std::string Strings::Heading = "Grammagic";
const std::string Strings::Stamina = "Stamina";
const std::string Strings::Start = "Start";
const std::string Strings::Quit = "Quit";

using namespace Magic;
using namespace Play;
namespace Core
{
    class Grammar
    {
        public:
            Grammar()
            {};

            ~Grammar()
            {};

            void start()
            {
                // Initialise randomness.
                srand(time(0));

                SDL_Init(SDL_INIT_EVERYTHING);
                int imgFlags = IMG_INIT_PNG|IMG_INIT_JPG|IMG_INIT_TIF;
                IMG_Init(imgFlags);
                TTF_Init();
                SDL_Window* window = SDL_CreateWindow("Grammage", 0, 0, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
                SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
                AssetCache assets = AssetCache(renderer);

                TitleStateManager title = TitleStateManager(renderer, &assets);
                PlayStateManager play = PlayStateManager(renderer, &assets);

                CoreState state = CoreState::Title;
                while(state != CoreState::Exit)
                {
                    SDL_Event e;
                    while(SDL_PollEvent(&e) != 0)
                    {
                        if (e.type == SDL_QUIT)
                        {
                            state = CoreState::Exit;
                        }
                    }
                    switch(state)
                    {
                        case CoreState::Title: {
                            state = title.start();
                            break;
                        }
                        case CoreState::Play: {
                            state = play.start();
                            break;
                        }
                        case CoreState::Exit:
                        default:
                            break;
                    }
                }
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                TTF_Quit();
                IMG_Quit();

                return;
            }
    };
}

int main ()
{
    Commands::allCommands.push_back(&Commands::HEAVIER);
    Commands::allCommands.push_back(&Commands::LIGHTER);
    Commands::allCommands.push_back(&Commands::HEAL);
    Commands::allCommands.push_back(&Commands::WEAKEN);
    Commands::allCommands.push_back(&Commands::SELF);
    Commands::allCommands.push_back(&Commands::SICKEST);
    Commands::allCommands.push_back(&Commands::ENEMY);
    Commands::allCommands.push_back(&Commands::FRESHEST);
    Commands::allCommands.push_back(&Commands::SLOW);
    Commands::allCommands.push_back(&Commands::HASTEN);
    Commands::allCommands.push_back(&Commands::FASTER);

    Core::Grammar program;
    program.start();

    return 0;
}
