#ifndef RUNE_H_INCLUDED
#define RUNE_H_INCLUDED

#include "menuItem.h"
#include "magic/word.h"

class Rune : public MenuItem
{
    public:
        Rune(Magic::Word*);

    private:
        Magic::Word* _word;

};

#endif
