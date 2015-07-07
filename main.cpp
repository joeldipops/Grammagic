#include <string>
#include "res/strings.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "util/assetCache.h"
#include "stateEnums.h"
#include "title/titleStateManager.h"
#include "play/playStateManager.h"

#include <stdlib.h>
#include <time.h>

const std::string Strings::Heading = "Grammagic";
const std::string Strings::Stamina = "Stamina";
const std::string Strings::Start = "Start";
const std::string Strings::Quit = "Quit";

    class Grammar
    {
        static const int WIDTH = 1200;
        static const int HEIGHT = 800;
        static const int BITS_PER_PIXEL = 32;

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

                Core::CoreState state = Core::CoreState::Title;
                while(state != Core::CoreState::Exit)
                {
                    SDL_Event e;
                    while(SDL_PollEvent(&e) != 0)
                    {
                        if (e.type == SDL_QUIT)
                        {
                            state = Core::CoreState::Exit;
                        }
                    }
                    switch(state)
                    {
                        case Core::CoreState::Title: {
                            state = title.start();
                            break;
                        }
                        case Core::CoreState::Play: {
                            state = play.start();
                            break;
                        }
                        case Core::CoreState::Exit:
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

int main ()
{
    Grammar program;
    program.start();

    return 0;
}
