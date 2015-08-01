#ifndef STATEMANAGER_H_INCLUDED
#define STATEMANAGER_H_INCLUDED

#include <SDL2/SDL.h>

#include "util/assetCache.h"
#include "globalConstants.h"

namespace Core
{
    template<typename State, typename ParentState>
    class StateManager
    {
        public:
            StateManager(SDL_Renderer* renderer, AssetCache* assets);

        protected:
            virtual const int moveCursor(const Core::InputPress, const int, const int, const int);
            virtual State state(void) const;
            virtual State state(State);

            virtual ParentState result(void) const ;
            virtual ParentState result(ParentState);

            SDL_Renderer* renderer(void);
            AssetCache* assets(void);

        private:
            SDL_Renderer* _renderer;
            AssetCache* _assets;
            State _state;
            ParentState _result;
    };
}

#endif
