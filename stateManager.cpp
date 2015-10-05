#include "stateManager.h"

using namespace Core;
using namespace Util;

/**
 * Constructor
 */
template <typename S, typename P>
StateManager<S, P>::StateManager(SDL_Renderer* renderer, AssetCache* assets)
{
    _renderer = renderer;
    _assets = assets;
}

/**
 * Gets and sets the current internal state.
 */
template <typename S, typename P>
S StateManager<S, P>::state(void) const
{
    return _state;
}
template <typename S, typename P>
S StateManager<S, P>::state(S state_)
{
    _state = state_;
    return _state;
}

/**
 * Gets and sets the current parent state
 */
template <typename S, typename P>
P StateManager<S, P>::result(void) const
{
    return _result;
}
template <typename S, typename P>
P StateManager<S, P>::result(P result_)
{
    _result = result_;
    return _result;
}

/**
 * Gets the SDL renderer.
 */
template <typename S, typename P>
SDL_Renderer* StateManager<S, P>::renderer(void)
{
    return _renderer;
}

/**
 * Gets the asset cache.
 */
template <typename S, typename P>
AssetCache* StateManager<S, P>::assets(void)
{
    return _assets;
}

/**
 * Moves the cursor around a menu.
 */
template <typename S, typename P>
const int StateManager<S, P>::moveCursor(const Core::InputPress input, const int selectedIndex, const int numberOfItems, const int itemsPerColumn)
{
    int index = selectedIndex;
    switch(input)
    {
        case Core::InputPress::UP:
            if (index <= 0)
                return selectedIndex;
            index--;
            break;
        case Core::InputPress::LEFT:
            index -= itemsPerColumn;
            if (index < 0)
                return selectedIndex;
            break;
        case Core::InputPress::DOWN:
            if (index >= numberOfItems - 1)
                return selectedIndex;
            index++;
            break;
        case Core::InputPress::RIGHT:
            index += itemsPerColumn;
            if (index >= numberOfItems)
                return selectedIndex;
            break;
    }

    return index;
}

template class StateManager<Play::PlayState, Core::CoreState>;
template class StateManager<Title::TitleState, Core::CoreState>;
template class StateManager<Play::MenuState, Play::PlayState>;
template class StateManager<Play::PlayState, Play::PlayState>;
