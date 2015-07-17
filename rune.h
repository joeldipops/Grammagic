#ifndef RUNE_H_INCLUDED
#define RUNE_H_INCLUDED

#include "menuItem.h"
#include "magic/word.h"

class Rune : public MenuItem
{
    public:
        Rune() {};
        Rune(Magic::Word*);
        virtual ~Rune(void);

    private:
        Magic::Word* _word;

};

#endif
