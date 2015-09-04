#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <time.h>

#include "res/strings.h"
#include "util/assetCache.h"
#include "globalConstants.h"
#include "title/titleStateManager.h"
#include "play/playStateManager.h"
#include "play/pc.h"
#include "persistence/saveLoad.h"
#include "res/battleCommands.h"
#include "res/templates.h"

const std::string Strings::Heading = "Grammagic";
const std::string Strings::Stamina = "Stamina";
const std::string Strings::Start = "Start";
const std::string Strings::Quit = "Quit";
const std::string Strings::Continue = "Continue";
const std::string Strings::Save = "Save";
const std::string Strings::SaveComplete = "Save Complete";
const std::string Strings::Magic = "Magic";
const std::string Strings::Party = "Party";
const std::string Strings::AName = "Aayee";
const std::string Strings::BName = "Beaey";
const std::string Strings::CName = "Cie";
const std::string Strings::HutDescription = "A safe place to rest";
const std::string Strings::GlassCannon = "Glass Cannon";
const std::string Strings::Tank = "Tank";
const std::string Strings::WellSpring = "Well Spring";
const std::string Strings::Wasp = "Wasp";
const std::string Strings::Guard = "Guard";
const std::string Strings::Jack = "Jack";

using namespace Magic;
using namespace Play;
using namespace Persistence;
using namespace Templates;
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

                // Initialise SDL
                SDL_Init(SDL_INIT_EVERYTHING);
                int imgFlags = IMG_INIT_PNG|IMG_INIT_JPG|IMG_INIT_TIF;
                IMG_Init(imgFlags);
                TTF_Init();
                SDL_Window* window = SDL_CreateWindow("Grammage", 0, 0, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
                SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
                AssetCache assets = AssetCache(renderer);

                // Initialise State Managers.
                TitleStateManager title = TitleStateManager(renderer, &assets);
                PlayStateManager play(renderer, &assets);

                SaveLoad io = SaveLoad(SAVE_FILE);
                CoreState state = CoreState::Title;
                while(state != CoreState::Exit)
                {
                    Party player = Party();
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
                        case CoreState::Load: {
                            io.load(player);

                            // Load was not successful.
                            if (player.members().size() <= 0)
                                state = CoreState::Play;
                            else
                                state = play.start(player);
                            break;
                        }
                        case CoreState::Play: {
                            player.addMember(Templates::Data::A);
                            player.addMember(Templates::Data::B);
                            player.addMember(Templates::Data::C);
                            state = play.start(player);
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
    Core::Grammar program;
    program.start();

    return 0;
}
