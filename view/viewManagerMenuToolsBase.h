#ifndef VIEWMANAGERMENUTOOLSBASE_H_INCLUDED
#define VIEWMANAGERMENUTOOLSBASE_H_INCLUDED

#include "viewManager.h"
namespace View
{
    class ViewManagerMenuToolsBase : public ViewManager
    {
        public:
            using ViewManager::ViewManager;
        protected:
            void renderPCs(const Play::Party&, int, const SDL_Rect&, int = -1, bool = false);

            static const int cursorXOffset;
            static const int cursorYOffset;
            static const SDL_Rect _menuControl;
            static const int borderWidth = 5;
            static const int marginLeft = 40;
    };
}
#endif
