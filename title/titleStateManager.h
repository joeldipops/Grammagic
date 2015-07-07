#ifndef INTROSTATEMANAGER_H_INCLUDED
#define INTROSTATEMANAGER_H_INCLUDED

#include "../menuItem.h"
#include "../title/titleViewManager.h"
#include "../stateManager.h"

class TitleStateManager final : StateManager<Title::TitleState, Core::CoreState>
{
    public:
        TitleStateManager(SDL_Renderer*, AssetCache*);
        ~TitleStateManager(void);
        Core::CoreState start(void);

    private:
        static const MenuItem START;
        static const MenuItem QUIT;
        std::vector<MenuItem> _menu;

        void render(void);
        bool moveCursor(const Core::InputPress);
        bool processCommand(void);

        TitleViewManager _view;
        int _selectedItemIndex;
};

#endif
