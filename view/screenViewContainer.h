#ifndef SCREENVIEWCONTAINER_H
#define SCREENVIEWCONTAINER_H

#include "viewManager.h"

namespace View
{
    struct ScreenViewContainer
    {
        public:
            ViewManager* Top;
            ViewManager* TopRight;
            ViewManager* Right;
            ViewManager* Main;
    };
}

#endif
