#include "rune.h"

Rune::Rune(Magic::Word* word_)
    : MenuItem(word_->name())
{
    _word = word_;
}

Rune::~Rune(void) {}

