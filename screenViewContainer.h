#ifndef SCREENVIEWCONTAINER_H
#define SCREENVIEWCONTAINER_H

#include "viewManager.h"

struct ScreenViewContainer
{
    public:
        ViewManager* Top;
        ViewManager* TopRight;
        ViewManager* Right;
        ViewManager* Main;

};

#endif
