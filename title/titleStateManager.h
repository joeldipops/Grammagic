#ifndef INTROSTATEMANAGER_H_INCLUDED
#define INTROSTATEMANAGER_H_INCLUDED

#include "../menuItem.h"
#include "../view/titleViewManager.h"
#include "../stateManager.h"
#include "../util/utils.h"
#include "../res/strings.h"

using namespace Core;
namespace Play
{
    class TitleStateManager final : Core::StateManager<Title::TitleState, Core::CoreState>
    {
        public:
            TitleStateManager(SDL_Renderer*, Util::AssetCache*);
            ~TitleStateManager(void);
            Core::CoreState start(void);

        private:
            static const MenuItem START;
            static const MenuItem QUIT;
            static const MenuItem CONTINUE;
            std::vector<MenuItem> _menu;

            void render(void);
            bool moveCursor(const Core::InputPress);
            bool processCommand(void);

            View::TitleViewManager _view;
            int _selectedItemIndex;
    };
}
#endif
