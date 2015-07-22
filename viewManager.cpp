#include "viewManager.h"

using namespace Util;

const SDL_Colour ViewManager::hudColour = { 0x19, 0x19, 0x70, 0xFF };
const SDL_Colour ViewManager::borderColour = { 0x69, 0x69, 0x69, 0xFF };
const SDL_Colour ViewManager::textColour = { 0xFF, 0xFF, 0xFF, 0xFF };
const SDL_Colour ViewManager::invisible = { 0xFF, 0xFF, 0xFF, 0x00 };
const SDL_Rect ViewManager::_control = SDL_Rect { 15, 15, 200, 55 };
const SDL_Rect ViewManager::messageBoxOuter = SDL_Rect { WIDTH / 2 - 150, (HEIGHT / 2) - 200, 300, 150 };
const SDL_Rect ViewManager::messageBoxInner = SDL_Rect { 25, 10, 200, 75 };;

const int ViewManager::controlBorderWidth = 5;
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


void ViewManager::drawHorizontalControls(
    const std::vector<MenuItem*>* items, const int selectedIndex, const SDL_Rect* port, const SDL_Rect* control
)
{
    SDL_Rect view;
    if (port == nullptr)
        view = viewPort();
    else
        view = *port;

    if (control == nullptr)
        control = &_control;

    SDL_Rect rect = SDL_Rect {view.x + control->x, view.y + control->y, control->w, control->h};
    SDL_Rect temp;

    int rows = 1;
    int i = 0;
    for(; i < int(items->size()); i++)
    {
        if (i == selectedIndex)
            drawOptionBox(&rect, items->at(i)->name(), 5, &hudColour, &textColour, &textColour);
        else
            drawOptionBox(&rect, items->at(i)->name(), 5, &hudColour, &borderColour, &borderColour);
        temp = SDL_Rect {rect.x + control->w + control->x, rect.y, control->w, control->h};

        // Can't fit horizontally, so shift below.
        if (temp.x + temp.w > view.x + view.w)
        {
            rows++;
            temp = SDL_Rect {view.x + control->x, control->y+ rect.y + rect.h, control->w, control->h };
        }

        rect = temp;
    }
}

/**
 * Displays a message box.
 * @param The message.
 * @param outer The outer boundary of the box.
 * @param inner The actual position of the text inside the box, scoped to within the outer box.
 * @param borderWidth The width of the border on the outer boundary.
 */
void ViewManager::drawMessage(const std::string message, const SDL_Rect& outer, const SDL_Rect& inner, int borderWidth)
{
    SDL_Rect rect = SDL_Rect { outer.x + inner.x, outer.y + inner.y, inner.w, inner.h };

    drawOptionBox(&rect, message, 0, &hudColour, &textColour, &invisible);
    drawBorder(outer, borderWidth, &borderColour, true);
}

void ViewManager::drawControls(const std::vector<MenuItem*>* items, const int selectedIndex, const SDL_Rect* port, const SDL_Rect* control)
{
    std::vector<const MenuItem*> cItems = std::vector<const MenuItem*>(0);
    cItems.reserve(items->size());
    for (unsigned int i = 0; i < items->size(); i++)
    {
        const MenuItem* n = items->at(i);
        cItems.push_back(n);
    }

    return drawControls(&cItems, selectedIndex, port, control);
}

/**
 * Render all the items that a player can pick from the menu.
 * @param items The list of items on the menu.
 * @param selectedIndex The position in the list of the currently selected item.
 */
void ViewManager::drawControls(const std::vector<const MenuItem*>* items, const int selectedIndex, const SDL_Rect* port, const SDL_Rect* control)
{
    SDL_Rect view;
    if (port == nullptr)
        view = viewPort();
    else
        view = *port;

    if (control == nullptr)
        control = &_control;

    SDL_Rect rect = SDL_Rect {view.x + control->x, view.y + control->y, control->w, control->h};
    SDL_Rect temp;

    int rows = 1;
    int i = 0;
    for(; i < int(items->size()); i++)
    {
        SDL_Colour textClr = items->at(i)->colour();
        if (i == selectedIndex)
            drawOptionBox(&rect, items->at(i)->name(), 5, &hudColour, &textClr, &textColour);
        else
            drawOptionBox(&rect, items->at(i)->name(), 5, &hudColour, &textClr, &borderColour);
        temp = SDL_Rect {rect.x, rect.y + control->h + control->y, control->w, control->h};

        // Can't fit vertically, so shift to the right.
        if (temp.y + temp.h > view.h + view.y)
        {
            rows++;
            temp = SDL_Rect { control->x + rect.x + rect.w, view.y + control->y, control->w, control->h };
        }

        rect = temp;
    }

    _menuItemsPerColumn = ceil(i / double(rows));
}

/**
 * Creates a texture to display use text, using the main font.
 * @param text The text to display.
 * @param colour The colour the text should be displayed as.
 * @return The created texture.
 */
SDL_Texture* ViewManager::formatFontTexture(std::string text, const SDL_Colour* colour)
{
    return _assets->get("res/font.ttf", text.c_str(), 30, *colour);
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
void ViewManager::drawBorder(const SDL_Rect& rect, int width, const SDL_Colour* colour = nullptr, bool willFillInwards = true)
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

    delete[] rects;
}

/**
 * Helper for drawing circles.
 */
void ViewManager::addToQuad(std::vector<std::vector<Util::Location>>& quads, int cx, int cy, int x, int y)
{
        int q = 0;
        if (x >= cx && y >= cy)
            q = 0;
        else if ( x >= cx && y < cy)
            q = 1;
        else if (x < cx && y < cy)
            q = 2;
        else
            q = 3;

        quads.at(q).push_back(Location(x, y));
};

void ViewManager::drawSector(int icx, int icy, int r, int startDegree, int endDegree)
{
    double error = (double)-r;
    double x = (double)r -0.5;
    double y = (double) 0.5;
    double cx = (double) icx - 0.5;
    double cy = (double) icy - 0.5;

    std::vector<std::vector<Util::Location>> quads = std::vector<std::vector<Location>>(4);

    while (x >= y)
    {
        addToQuad(quads, cx, cy, cx + x, cy + y);
        addToQuad(quads, cx, cy, cx + y, cy + x);

        if (x != 0)
        {
            addToQuad(quads, cx, cy, cx - x, cy + y);
            addToQuad(quads, cx, cy, cx + y, cy - x);
        }

        if (y != 0)
        {
            addToQuad(quads, cx, cy, cx + x, cy - y);
            addToQuad(quads, cx, cy, cx - y, cy + x);
        }

        if (x != 0 && y != 0)
        {
            addToQuad(quads, cx, cy, cx - x, cy - y);
            addToQuad(quads, cx, cy,  cx - y, cy - x);
        }

        error += y;
        ++y;
        error += y;

        if (error >= 0)
        {
            --x;
            error -= x;
            error -= x;
        }
    }

    std::sort(quads[0].begin(), quads[0].end(), [](Location lhs, Location rhs)->bool
    {
        return lhs.X < rhs.X;
    });

    std::sort(quads[1].begin(), quads[1].end(), [](Location lhs, Location rhs)->bool
    {
        return lhs.X > rhs.X;
    });

    std::sort(quads[2].begin(), quads[2].end(), [](Location lhs, Location rhs)->bool
    {
        return lhs.X > rhs.X;
    });

    std::sort(quads[3].begin(), quads[3].end(), [](Location lhs, Location rhs)->bool
    {
        return lhs.X < rhs.X;
    });


    int totalData = quads[0].size() + quads[1].size() + quads[2].size() + quads[3].size();

    int start = totalData * ((startDegree % 360) / 360.0);
    int finish = totalData * ((endDegree % 360) / 360.0);
    int quadBoundary = totalData / 4;

    int i = start;
    while (i != finish)
    {
        int quadrant = i / quadBoundary;
        int j = i % quadBoundary;

        i++;

        if (j == 0 && quadrant != 0)
            continue;

        if (j >= int(quads[quadrant].size()))
            continue;

        Location loc = quads[quadrant][j];

        SDL_RenderDrawLine(_renderer, cx, cy, loc.X, loc.Y);

        if (i >= totalData)
            i = 0;
    }
}
