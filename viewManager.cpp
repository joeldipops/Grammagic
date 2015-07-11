#include "viewManager.h"

const SDL_Colour ViewManager::hudColour = { 0x19, 0x19, 0x70, 0xFF };
const SDL_Colour ViewManager::borderColour = { 0x69, 0x69, 0x69, 0xFF };
const SDL_Colour ViewManager::textColour = { 0xFF, 0xFF, 0xFF, 0xFF };
const int ViewManager::DEFAULT_BORDER_WIDTH = 7;

/**
 * Constructor
 * @param renderer SDL Renderer for drawing within the view port.
 * @param viewPort the size and position of this viewport within the window.
 * @param assets Image (later sound, font, etc) assets shared between classes.
 */
ViewManager::ViewManager(SDL_Renderer* renderer, SDL_Rect viewPort, AssetCache* assets)
{
    _renderer = renderer;
    _viewPort = viewPort;
    _assets = assets;
}

/**
 * Destructor
 */
ViewManager::~ViewManager(void)
{
    _renderer = nullptr;
    _assets = nullptr;
}

/**
 * Renders visual elements within the given view port.  Must be overriden for specific visuals.
 */
void ViewManager::render()
{
    _menuItemsPerColumn = 0;
    SDL_RenderSetViewport(_renderer, &_viewPort);
}

/**
 * @return the renderer object for use by sub classes
 */
SDL_Renderer* ViewManager::renderer(void)
{
    return _renderer;
}

/**
 * @return the viewport rectangle for use by sub classes
 */
SDL_Rect ViewManager::viewPort(void)
{
    return _viewPort;
}

/**
 * @return the asset cache for use by sub classes.
 */
AssetCache* ViewManager::assets(void)
{
    return _assets;
}

/**
 * If a menu is being used, gives how many items vertically it has.
 * @return Number of menu items per vertical column.
 */
int ViewManager::menuItemsPerColumn(void) const
{
    return _menuItemsPerColumn;
}


/**
 * Render all the items that a player can pick from the menu.
 * @param items The list of items on the menu.
 * @param selectedIndex The position in the list of the currently selected item.
 */
void ViewManager::drawControls(const std::vector<MenuItem*>* items, const int selectedIndex, const SDL_Rect* port)
{
    const int marginTop = 15;
    const int marginLeft = 15;
    const int width = 200;
    const int height = 55;

    SDL_Rect view;
    if (port == nullptr)
        view = viewPort();
    else
        view = *port;

    SDL_Rect rect = SDL_Rect {view.x + marginLeft, view.y + marginTop, width, height};
    SDL_Rect temp;

    int rows = 1;
    int i = 0;
    for(; i < int(items->size()); i++)
    {
        if (i == selectedIndex)
            drawOptionBox(&rect, items->at(i)->name(), 5, &hudColour, &textColour, &textColour);
        else
            drawOptionBox(&rect, items->at(i)->name(), 5, &hudColour, &borderColour, &borderColour);
        temp = SDL_Rect {rect.x, rect.y + height + marginTop, width, height};

        // Can't fit horizontally, so shift to the right.
        if (temp.y + temp.h > view.h + view.y)
        {
            rows++;
            temp = SDL_Rect {marginLeft + rect.x + rect.w, view.y + marginTop, width, height };
        }

        rect = temp;
    }

    _menuItemsPerColumn = ceil(i / float(rows));
}

/**
 * Creates a texture to display use text, using the main font.
 * @param text The text to display.
 * @param colour The colour the text should be displayed as.
 * @return The created texture.
 */
SDL_Texture* ViewManager::formatFontTexture(std::string text, const SDL_Colour* colour)
{
    TTF_Font* font = _assets->get("res/font.ttf", 30);
    return SDL_CreateTextureFromSurface(_renderer, TTF_RenderText_Solid(font, text.c_str(), *colour));
}


/**
 * Render a box with a border and text in the middle.
 */
void ViewManager::drawOptionBox(const SDL_Rect* rect, const std::string text, int borderWidth, const SDL_Colour* bgColour, const SDL_Colour* fgColour, const SDL_Colour* borderColour)
{
    // Background
    SDL_SetRenderDrawColor(_renderer, bgColour->r, bgColour->g, bgColour->b, bgColour->a);
    SDL_RenderFillRect(_renderer, rect);

    // Foreground
    int textOffset = borderWidth * 2;
    SDL_Texture* texTure = formatFontTexture(text, fgColour);
    SDL_Rect textArea = SDL_Rect { rect->x + textOffset, rect->y + textOffset, rect->w - textOffset * 2, rect->h - textOffset * 2};
    SDL_RenderCopy(_renderer, texTure, NULL, &textArea);

    // Border
    drawBorder(*rect, borderWidth, borderColour, true);
}

/**
* Fills the view port with a rect, starting from the top left.
* @param colour The rectangle should either be this or the current colour.
*/
void ViewManager::fillViewport(const SDL_Colour* colour = nullptr)
{
    if (colour != nullptr)
        SDL_SetRenderDrawColor(_renderer, colour->r, colour->g, colour->b, colour->a);
    SDL_Rect rect = SDL_Rect { 0, 0 ,_viewPort.w, _viewPort.h };
    SDL_RenderFillRect(_renderer, &rect);
};

/**
 * Draws a border around the viewport
 * @param width The width of the border.
 * @param colour The colour of the border.
 */
void ViewManager::drawBorder(int width, const SDL_Colour* colour = nullptr)
{
    SDL_Rect rect = {0, 0, _viewPort.w, _viewPort.h};
    drawBorder(rect, width, colour, true);
}

/**
 * Draws a border of the area set by rect.
 * @param rect Draw a border around this rect.
 * @param width The width of the border.
 * @param colour The colour of the border.
 * @param willFillInwards If true, the border will appear wholly inside the rectangle.
 *                        If false, the border will wrap outside the rectangle.
 */
void ViewManager::drawBorder(SDL_Rect rect, int width, const SDL_Colour* colour = nullptr, bool willFillInwards = true)
{
    if (colour != nullptr)
        SDL_SetRenderDrawColor(_renderer, colour->r, colour->g, colour->b, colour->a);

    int top;
    int bottom;
    int left;
    int right;

    if (willFillInwards)
    {
        // If width is 1, just use the native function.  This should be faster.
        if (width == 1) {
            SDL_RenderDrawRect(_renderer, &rect);
            return;
        }


        top = rect.y;
        bottom = rect.y + rect.h;
        left = rect.x;
        right = rect.x + rect.w;
    }
    else
    {
        top = rect.y - width;
        bottom = top + rect.h + width;
        left = rect.x - width;
        right = left + rect.w + width;
    }

    SDL_Rect topSide = {
        left, top,
        right - left,
        width
    };

    SDL_Rect leftSide = {
        left, top,
        width,
        bottom - top
    };

    SDL_Rect bottomSide = {
        left,
        bottom - width,
        right - left,
        width
    };

    SDL_Rect rightSide = {
        right - width,
        top,
        width,
        bottom - top
    };

    SDL_Rect** rects = new SDL_Rect*[4];
    rects[0] = &topSide;
    rects[1] = &leftSide;
    rects[2] = &bottomSide;
    rects[3] = &rightSide;

    SDL_RenderFillRects(_renderer, *rects, 4);
}
