#ifndef CONTROLVIEWMANAGER_H_INCLUDED
#define CONTROLVIEWMANAGER_H_INCLUDED

#include "viewManager.h"

namespace Play { class Mob; }

namespace View
{
    class ControlViewManager : public ViewManager
    {
        public:
            ControlViewManager(SDL_Renderer*, SDL_Rect, Util::AssetCache*);
            void render(const Play::Mob*, Play::PlayState, const std::string& = "");
            natural lastDrawnCharCount(void) const;

        private:
            static const SDL_Rect _runeControl;

            void writeHeading(void);
            void writeMessage(const std::string&);
            void drawOptionBox(const SDL_Rect&,  const Core::MenuItem*, int borderWidth, const SDL_Colour& bgColour, const SDL_Colour& fgColour, const SDL_Colour& textColour);
            natural _lastDrawnCharCount;
    };
}

#endif
