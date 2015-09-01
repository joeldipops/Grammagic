#include "adverb.h"

using namespace Magic;

//{Lifecycle
Adverb::Adverb(std::string n, Modifier e, Modifier c, Modifier d)
    :Word(n, e, c, d)
{}

Adverb::Adverb(Rune* adv)
    :Word(std::vector<Rune*>{adv})
{
    if (!adv->isAdverb())
        throw;

    _modify = adv->metaActionFn();
}
//}

//{Methods
SpellData& Adverb::modifySpell(SpellData& data)
{
    return _modify(data);
}

WordType Adverb::type(void) const
{
    return WordType::ADVERB;
}
//}

