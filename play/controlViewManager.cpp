#include "viewManagers.h"

ControlViewManager::ControlViewManager(SDL_Renderer* r, SDL_Rect v, AssetCache* a) : ViewManager(r, v, a) {}

void ControlViewManager::render(const Mob* pc, Play::PlayState state)
{
    ViewManager::render();
    fillViewport(&hudColour);
    switch(state)
    {
        case Play::PlayState::Movement:
            writeHeading(); break;
        case Play::PlayState::Combat: {
            std::vector<Command> commands = pc->commands();
            std::vector<Command*> pointers = toPointers(commands);
            std::vector<MenuItem*> menuItems = std::vector<MenuItem*> {pointers.begin(), pointers.end()};

            if (pc->isBlocked())
                drawControls(&menuItems, -1);
            else
                drawControls(&menuItems, pc->selectedCommandIndex());
            break;
        }
        default: break;

    }

    drawBorder(DEFAULT_BORDER_WIDTH, &borderColour);
}

void ControlViewManager::writeHeading(void)
{
    SDL_Texture* texture = formatFontTexture(Strings::Heading, &textColour);
    SDL_Rect textArea = SDL_Rect { viewPort().x + 20, viewPort().y + 5, viewPort().w - 40, viewPort().h - 10 };
    SDL_RenderCopy(renderer(), texture, NULL, &textArea);
}
