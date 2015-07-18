#ifndef RUNE_H_INCLUDED
#define RUNE_H_INCLUDED

#include "menuItem.h"
#include "magic/word.h"
#include "globalConstants.h"

class Rune : public MenuItem
{
    public:
        Rune() {};
        Rune(Magic::Word*);
        virtual ~Rune(void);
        SDL_Colour colour(void) const;

    private:
        Magic::Word* _word;

        static const SDL_Colour VERB_COLOUR;
        static const SDL_Colour NOUN_COLOUR;
        static const SDL_Colour ADJECTIVE_COLOUR;
        static const SDL_Colour ADVERB_COLOUR;
};

#endif
