#ifndef MENURUNE_H_INCLUDED
#define MENURUNE_H_INCLUDED

#include "menuItem.h"
#include "magic/word.h"
#include "globalConstants.h"

class MenuRune : public MenuItem
{
    public:
        MenuRune() {};
        MenuRune(Magic::Word*);
        virtual ~MenuRune(void);
        SDL_Colour colour(void) const;

    private:
        Magic::Word* _word;

        static const SDL_Colour VERB_COLOUR;
        static const SDL_Colour NOUN_COLOUR;
        static const SDL_Colour ADJECTIVE_COLOUR;
        static const SDL_Colour ADVERB_COLOUR;
};

#endif
