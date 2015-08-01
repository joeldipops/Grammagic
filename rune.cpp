#include "rune.h"

using namespace Magic;

const SDL_Colour Rune::VERB_COLOUR = SDL_Colour { 0xFF, 0xFF, 0x00, 0xFF };
const SDL_Colour Rune::NOUN_COLOUR = SDL_Colour { 0x00, 0xFF, 0xFF, 0xFF };
const SDL_Colour Rune::ADJECTIVE_COLOUR = SDL_Colour { 0xFF, 0x00, 0x88, 0xFF };
const SDL_Colour Rune::ADVERB_COLOUR = SDL_Colour { 0x88, 0xFF, 0xFF, 0xFF };

Rune::Rune(Word* word_)
    : MenuItem(word_->name())
{
    _word = word_;
}

Rune::~Rune(void) {}

/**
 * Returns a colour based on the contained word type.
 */
SDL_Colour Rune::colour(void) const
{
    switch(_word->type())
    {
        case WordType::AVerb:
            return VERB_COLOUR;
        case WordType::ANoun:
        case WordType::ANounPhrase:
            return NOUN_COLOUR;
        case WordType::AnAdjective:
            return ADJECTIVE_COLOUR;
        case WordType::AnAdverb:
            return ADVERB_COLOUR;
        default:
            return MenuItem::colour();
    }
}

