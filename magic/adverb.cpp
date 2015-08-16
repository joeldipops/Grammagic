#include "adverb.h"

using namespace Magic;

Adverb::Adverb(std::string n, Modifier e, Modifier c, Modifier d)
    :Word(n, e, c, d)
{}


WordType Adverb::type(void) const
{
    return WordType::ADVERB;
}

