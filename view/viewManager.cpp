#include "viewManager.h"
#include "../util/assetCache.h"
#include "../menuItem.h"

using namespace View;

using namespace Core;
using namespace Util;


// Constants
//{
const SDL_Colour ViewManager::BG_COLOUR = { 0x19, 0x19, 0x70, 0xFF };
const SDL_Colour ViewManager::TEXT_COLOUR = { 0x69, 0x69, 0x69, 0xFF };
const SDL_Colour ViewManager::SELECTED_COLOUR = { 0xFF, 0xFF, 0xFF, 0xFF };
const SDL_Colour ViewManager::HIGHLIGHTED_COLOUR = { 0xFF, 0xFF, 0x00, 0xFF };
const SDL_Colour ViewManager::INVISIBLE = { 0xFF, 0xFF, 0xFF, 0x00 };
const SDL_Rect ViewManager::_control = SDL_Rect { 15, 15, 200, 55 };
const SDL_Rect ViewManager::messageBoxOuter = SDL_Rect { WIDTH / 2 - 150, (HEIGHT / 2) - 200, 300, 150 };
const SDL_Rect ViewManager::letterSize = SDL_Rect { 0, 0, 20, 40 };

const int ViewManager::controlBorderWidth = 5;
const int ViewManager::DEFAULT_BORDER_WIDTH = 7;
//}

//{LifeCycle
/**
 * Constructor
 * @param renderer SDL Renderer for drawing within the view port.
 * @param viewPort the size and position of this viewport within the window.
 * @param assets Image (later sound, font, etc) assets shared between classes.
 */
ViewManager::ViewManager(SDL_Renderer* renderer, SDL_Rect& viewPort, AssetCache* assets)
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

//}

//{Properties
/**
 * @return the renderer object for use by sub classes
 */
SDL_Renderer* ViewManager::renderer(void) { return _renderer; }

/**
 * @return a copy of the viewport rectangle for use by sub classes
 */
SDL_Rect ViewManager::viewPort(void) const { return _viewPort; }

/**
 * @return the asset cache for use by sub classes.
 */
AssetCache* ViewManager::assets(void) { return _assets; }

/**
 * If a menu is being used, gives how many items vertically it has.
 * @return Number of menu items per vertical column.
 */
int ViewManager::menuItemsPerColumn(void) const { return _menuItemsPerColumn; }
//}

//{Methods
/**
 * Renders visual elements within the given view port.  Must be overriden for specific visuals.
 */
void ViewManager::render()
{
    _menuItemsPerColumn = 0;
    SDL_RenderSetViewport(_renderer, &_viewPort);
}


void ViewManager::drawHorizontalControls(
    const std::vector<MenuItem*>& items, const int selectedIndex,
    const SDL_Rect* port, const SDL_Rect* control, const SDL_Colour* unselectedColour, const SDL_Colour* selectedColour
)
{
    SDL_Rect view;
    if (port == nullptr)
        view = viewPort();
    else
        view = *port;

    if (control == nullptr)
        control = &_control;

    const SDL_Colour& usColour = (unselectedColour != nullptr) ? *unselectedColour : TEXT_COLOUR;
    const SDL_Colour& sColour = (selectedColour != nullptr) ? *selectedColour : SELECTED_COLOUR;

    SDL_Rect rect = SDL_Rect {view.x + control->x, view.y + control->y, control->w, control->h};
    SDL_Rect temp;

    int rows = 1;
    int i = 0;
    for(; i < int(items.size()); i++)
    {


        if (i == selectedIndex)
            drawOptionBox(rect, items.at(i), 5, BG_COLOUR, sColour, sColour);
        else
            drawOptionBox(rect, items.at(i), 5, BG_COLOUR, usColour, usColour);
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
 * Displays text to the screen.
 * @param message The text to display
 * @param letterSize The average size of one letter or space.
 * @param maximum The maximum amount of area allowed for the message.
 * @param border If true, a border will be drawn around the message.
 * @return The number of characters shown.
 */
natural ViewManager::drawMessage(const std::string& message, const SDL_Rect& letterSize, const SDL_Rect& maximum, bool showBorder)
{
    natural result = 0;
    const int paddingX = 10;
    const int paddingY = 5;
    SDL_Rect outer { maximum.x, maximum.y, 0, 0};
    SDL_Rect inner { maximum.x + paddingX, maximum.y + paddingY, 0, 0 };

    std::vector<std::string> lines = Util::split(message, '\n');

    if (lines.size() <= 1 && int(message.length() * letterSize.w) < maximum.w)
    {
        inner.w = message.length() * letterSize.w;
        inner.h = letterSize.h;

        outer.w = inner.w + paddingX * 2;
        outer.h = inner.h + paddingY * 2;

        drawOptionBox(inner, message, 0, BG_COLOUR, SELECTED_COLOUR, SELECTED_COLOUR);
        result = message.length();
    }
    else
    {
        int maxX = maximum.x + maximum.w;
        int maxY = maximum.y + maximum.h;

        SDL_Rect rect { maximum.x + paddingX, maximum.y + paddingY, 0, letterSize.h };

        for (natural l = 0; l < lines.size(); l++)
        {
            std::string line = lines[l];
            std::vector<std::string> words = Util::split(line, ' ');
            if (words.size() <= 0) {
                result++;
                continue;
            }
            natural w = 0;
            for (;w < words.size(); w++)
            {
                std::string word = words[w];
                if (word.length() <= 0) {
                    result++;
                    continue;
                }

                int width = word.length() * letterSize.w;

                // Go down to the next line.
                if (rect.x + width > maxX)
                {
                    if (rect.x + width > outer.x + outer.w)
                        outer.w = rect.x - outer.x;

                    rect.y = rect.y + rect.h;
                    rect.x = maximum.x + paddingX;
                    if (rect.y + rect.h > maxY)
                        break;
                }

                rect.w = width;

                drawOptionBox(rect, word, 0, BG_COLOUR, INVISIBLE, SELECTED_COLOUR);

                rect.x = rect.x + rect.w + letterSize.w;
                result += word.length() + 1;
            }
            if (w == words.size())
                result--;
            rect.x = maximum.x + paddingX;
            rect.y = rect.y + rect.h;
            if (rect.y + rect.h > maxY)
                break;
            result++;
        }
        outer.h = rect.y + rect.h + paddingY - outer.y;
    }

    if (showBorder)
        drawBorder(outer, 3, &SELECTED_COLOUR, true);

    return result;
}

/**
 * Render all the items that a player can pick from the menu.
 * @param items The list of items on the menu.
 * @param selectedIndex The position in the list of the currently selected item.
 */
void ViewManager::drawControls(const std::vector<MenuItem*>& items, const int selectedIndex, const SDL_Rect* port, const SDL_Rect* control, bool allowSpilling)
{
    // Cast MenuItems to const before calling.
    std::vector<const MenuItem*> cItems = std::vector<const MenuItem*>(0);
    cItems.reserve(items.size());
    for (natural i = 0; i < items.size(); i++)
    {
        const MenuItem* n = items.at(i);
        cItems.push_back(n);
    }

    return drawControls(cItems, selectedIndex, port, control, allowSpilling);
}

/**
 * Render all the items that a player can pick from the menu.
 * @param items The list of items on the menu.
 * @param selectedIndex The position in the list of the currently selected item.
 */
void ViewManager::drawControls(const std::vector<const MenuItem*>& items, const int selectedIndex, const SDL_Rect* port, const SDL_Rect* control, bool allowSpilling)
{
    const int arrowOffset = 15;
    const int borderWidth = 5;
    SDL_Rect view;
    if (port == nullptr)
        view = viewPort();
    else
        view = *port;

    if (control == nullptr)
        control = &_control;

    SDL_Rect rect = SDL_Rect {view.x + control->x, view.y + control->y, control->w, control->h};

    int i = 0;
    int visiblePerPage = 0;
    if (allowSpilling)
    {
        // find number of buttons that can be shown
        visiblePerPage = ((view.w - arrowOffset)/ (control->w + control->x)) * (view.h / (control->h + control->y));
        if (visiblePerPage <= 0)
            return;

        if (selectedIndex >= visiblePerPage)
        {
            SDL_Texture* arrowLeft = _assets->get(RESOURCE_LOCATION + "arrow-left.png");
            SDL_Rect arrowRect = SDL_Rect { view.x + borderWidth + 2, view.y + (view.h / 2), 15, 15 };
            SDL_RenderCopy(_renderer, arrowLeft, NULL, &arrowRect);
            rect.x += arrowOffset;
        }
        if (int(items.size()) > visiblePerPage)
        {
            SDL_Texture* arrowRight = _assets->get(RESOURCE_LOCATION + "arrow-right.png");
            SDL_Rect arrowRect = SDL_Rect { view.x + view.w - borderWidth - 2 - 15, view.y + (view.h / 2), 15, 15 };
            SDL_RenderCopy(_renderer, arrowRight, NULL, &arrowRect);
        }
    }
    else
    {
        visiblePerPage = ((view.w)/ (control->w + control->x)) * (view.h / (control->h + control->y));
    }

    if (selectedIndex >= 0)
        i = (selectedIndex / visiblePerPage) * visiblePerPage;
    else
        i = 0;

    SDL_Rect temp;

    int limit = (i + visiblePerPage) >= int(items.size())
    ? items.size()
    : i + visiblePerPage;

    for(; i < limit; i++)
    {
        SDL_Colour textClr = items.at(i)->colour();
        if (i == selectedIndex)
            drawOptionBox(rect, items.at(i), borderWidth, BG_COLOUR, textClr, SELECTED_COLOUR);
        else
            drawOptionBox(rect, items.at(i), borderWidth, BG_COLOUR, textClr, TEXT_COLOUR);
        temp = SDL_Rect {rect.x, rect.y + control->h + control->y, control->w, control->h};

        // Can't fit vertically, so shift to the right.
        if (temp.y + temp.h > view.h + view.y)
        {
            if (!allowSpilling)
                break;
            temp = SDL_Rect { control->x + rect.x + rect.w, view.y + control->y, control->w, control->h };
        }

        rect = temp;
    }

    _menuItemsPerColumn = view.h / (control->h + control->y);
}

/**
 * Creates a texture to display use text, using the main font.
 * @param text The text to display.
 * @param colour The colour the text should be displayed as.
 * @return The created texture.
 */
SDL_Texture* ViewManager::formatFontTexture(std::string text, const SDL_Colour* colour)
{
    return _assets->get("fonts/font.ttf", text.c_str(), 30, *colour);
}

/**
 * Expresses a float with no more than two decimal places as an integer < 100
 */
std::string ViewManager::displayMultiplier(float value) const
{
    int result = int(value * 100);
    return std::to_string(result);
}


void ViewManager::drawOptionBox(const SDL_Rect& rect,  const MenuItem* item, int borderWidth, const SDL_Colour& bgColour, const SDL_Colour& fgColour, const SDL_Colour& textColour)
{
    SDL_Texture* texture = nullptr;

    if (item->imagePath().size() <= 0)
        texture = formatFontTexture(item->name(), &fgColour);
    else
        texture = assets()->get(item->imagePath());

    return drawOptionBox(rect, texture, borderWidth, bgColour, fgColour, textColour);
}

void ViewManager::drawOptionBox(const SDL_Rect& rect, const std::string& text, const int borderWidth, const SDL_Colour& bgColour, const SDL_Colour& fgColour, const SDL_Colour& textColour)
{
    SDL_Texture* texture = formatFontTexture(text, &fgColour);
    return drawOptionBox(rect, texture, borderWidth, bgColour, fgColour, textColour);
}

/**
 * Render a box with a border and text in the middle.
 */
void ViewManager::drawOptionBox(const SDL_Rect& rect, SDL_Texture* texture, int borderWidth, const SDL_Colour& bgColour, const SDL_Colour& fgColour, const SDL_Colour& textColour)
{
    // Background
    SDL_SetRenderDrawColor(_renderer, bgColour.r, bgColour.g, bgColour.b, bgColour.a);
    SDL_RenderFillRect(_renderer, &rect);

    // Foreground
    int textOffset = borderWidth * 2;
    SDL_Rect textArea = SDL_Rect { rect.x + textOffset, rect.y + textOffset, rect.w - textOffset * 2, rect.h - textOffset * 2};
    SDL_RenderCopy(_renderer, texture, NULL, &textArea);

    // Border
    drawBorder(rect, borderWidth, &textColour, true);
}

/**
* Fills the view port with a rect, starting from the top left.
* @param colour The rectangle should either be this or the current colour.
*/
void ViewManager::fillViewport(const SDL_Colour& colour)
{
    SDL_SetRenderDrawColor(_renderer, colour.r, colour.g, colour.b, colour.a);
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

/**
 * Draw a sector of a circle.
 */
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

//}
