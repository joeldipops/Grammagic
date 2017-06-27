#include "titleStateManager.h"
#include "../util/events.h"

using namespace Play;

using namespace Resources;
const MenuItem TitleStateManager::START = MenuItem(Strings::Start);
const MenuItem TitleStateManager::QUIT = MenuItem(Strings::Quit);
const MenuItem TitleStateManager::CONTINUE = MenuItem(Strings::Continue);

/**
 * Constructor
 */
TitleStateManager::TitleStateManager(SDL_Renderer* r, Util::AssetCache* a) : StateManager(r, a)
{
    _view = View::TitleViewManager(renderer(), SDL_Rect {0, 0, 1200, 800}, assets());
    _menu = std::vector<MenuItem> {START, CONTINUE, QUIT};
    state(Title::TitleState::Normal);
}

TitleStateManager::~TitleStateManager()
{

}

/**
 * Sets up graphics then Starts the main loop for this state.
 * @returns the state the core loop should be in when the PlayState ends.
 */
Core::CoreState TitleStateManager::start(void)
{
    state(Title::TitleState::Normal);
    result(Core::CoreState::Exit);
    bool rerender = true;
    _selectedItemIndex = 0;
    Title::TitleState oldState;
    while(state() != Title::TitleState::Exit)
    {
        if(rerender)
            render();
        else
            Util::sleep(50);

        oldState = state();

        Core::Event myEvent;
        while(eventManager.pollEvent(&myEvent) && oldState == state())
        {
            SDL_Event event = *myEvent.InnerEvent;
            switch(event.type)
            {
                case SDL_QUIT:
                    state(Title::TitleState::Exit);
                    result(Core::CoreState::Exit);
                    continue;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_q:
                        case SDLK_ESCAPE:
                            state(Title::TitleState::Exit);
                            continue;
                        case SDLK_w:
                            rerender = moveCursor(Core::InputPress::UP);
                            break;
                        case SDLK_a:
                            rerender = moveCursor(Core::InputPress::LEFT);
                            break;
                        case SDLK_s:
                            rerender = moveCursor(Core::InputPress::DOWN);
                            break;
                        case SDLK_d:
                            rerender = moveCursor(Core::InputPress::RIGHT);
                            break;
                        case SDLK_RETURN:
                        case SDLK_RIGHTBRACKET:
                            rerender = processCommand();
                            break;
                    }
                    break;
            }
        }
    }

    return result();
}

void TitleStateManager::render(void)
{
    std::vector<MenuItem*> pointers = Util::toPointers<MenuItem>(_menu);
    _view.render(pointers, _selectedItemIndex);
    SDL_RenderPresent(renderer());
}

bool TitleStateManager::moveCursor(const Core::InputPress input)
{
    int result = StateManager::moveCursor(input, _selectedItemIndex, _menu.size(), _menu.size());
    if (result != _selectedItemIndex)
    {
        _selectedItemIndex = result;
        return true;
    }

    return false;
}

bool TitleStateManager::processCommand(void)
{
    MenuItem command = _menu.at(_selectedItemIndex);

    if (command.equals(START))
    {
        state(Title::TitleState::Exit);
        result(Core::CoreState::Play);
    }
    else if (command.equals(QUIT))
    {
        state(Title::TitleState::Exit);
        result(Core::CoreState::Exit);
    }
    else if (command.equals(CONTINUE))
    {
        state(Title::TitleState::Exit);
        result(Core::CoreState::Load);
    }
    return false;
}
