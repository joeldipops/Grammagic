#include "viewManagers.h"

ControlViewManager::ControlViewManager(SDL_Renderer* r, SDL_Rect v, AssetCache* a) : ViewManager(r, v, a) {}

void ControlViewManager::render(const Mob* pc, Play::PlayState state, const std::string& message)
{
    _lastDrawnCharCount = 0;
    ViewManager::render();
    fillViewport(&BG_COLOUR);
    switch(state)
    {
        case Play::PlayState::Movement:
        case Play::PlayState::Message:
            if (message.size() > 0)
                writeMessage(message);
            else
                writeHeading();
            break;
        case Play::PlayState::Combat: {
            if (pc == nullptr)
                break;
            std::vector<Command*> commands = pc->commands();
            std::vector<MenuItem*> menuItems = std::vector<MenuItem*> {commands.begin(), commands.end()};

            if (pc->isBlocked())
                drawControls(menuItems, -1);
            else
                drawControls(menuItems, pc->selectedCommandIndex());
            break;
        }
        default: break;

    }

    drawBorder(DEFAULT_BORDER_WIDTH, &TEXT_COLOUR);
}

/**
 * Draws a message in the view, such as dialogue or information
 */
void ControlViewManager::writeMessage(const std::string& message)
{
    SDL_Rect vp = viewPort();
    vp.x = vp.x + 4;
    vp.y = vp.y + 4;
    vp.w = vp.w - 8;
    vp.h = vp.h - 8;
    SDL_Rect letterSize {0, 0, vp.h / 4, vp.h / 4};
    _lastDrawnCharCount = drawMessage(message, letterSize, viewPort(), false);
}

void ControlViewManager::writeHeading(void)
{
    SDL_Texture* texture = formatFontTexture(Strings::Heading, &SELECTED_COLOUR);
    SDL_Rect textArea = SDL_Rect { viewPort().x + 20, viewPort().y + 5, viewPort().w - 40, viewPort().h - 10 };
    SDL_RenderCopy(renderer(), texture, NULL, &textArea);
}

natural ControlViewManager::lastDrawnCharCount(void) const { return _lastDrawnCharCount; }
