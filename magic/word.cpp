#include "word.h"

using namespace Magic;

//{Lifecycle
/**
 * Constructor
 */
Word::Word(std::vector<Rune*> data)
{
    for (Rune* r : data)
    {
        _addCost += r->addCost();
        _addDuration += r->addDuration();
        _addEffect += r->addEffect();

        _modCost *= r->modCost();
        _modDuration *= r->modDuration();
        _modEffect *= r->modEffect();
    }
}

/**
 * Destructor
 */
Word::~Word(void) {}
//}

//{ Properties
int Word::addEffect(void) const { return _addEffect; }
int Word::addCost(void) const { return _addCost; }
int Word::addDuration(void) const { return _addDuration; }
float Word::modEffect(void) const { return _modEffect; }
float Word::modCost(void) const { return _modCost; }
float Word::modDuration(void) const { return _modDuration; }
//}
