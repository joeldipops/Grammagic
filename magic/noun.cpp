#include "noun.h"

using namespace Magic;

//{Friends
Combatable* Magic::properNounAcquire(Noun* context, Mob* caster, SpellContext& field, SpellData& data)
{
    return context->_target(caster, field, data);
};

Combatable* Magic::auxAdjNounAcquire(Noun* context, Mob* caster, SpellContext& field, SpellData& data)
{
    data.modality = context->_modality;
    std::vector<Combatable*> candidates = context->_findCandidates(caster, field, data);
    return (candidates.size() <= 0)
        ? nullptr
        : context->_select(caster, field, candidates, data);
};

Combatable* Magic::adjNounAcquire(Noun* context, Mob* caster, SpellContext& field, SpellData& data)
{
    std::vector<Combatable*> candidates = context->_findCandidates(caster, field, data);
    return (candidates.size() <= 0)
        ? nullptr
        : context->_select(caster, field, candidates, data);

    return context->_select(caster, field, candidates, data);
};

//}



//{Life Cycle
/**
 * Constructor
 */
Noun::Noun(Rune* properNoun)
    : Word(std::vector<Rune*> {properNoun})
{
    if (!properNoun->isProperNoun())
        throw;

    _target = properNoun->getTargetFn();
    _targetWrapper = properNounAcquire;
}

/**
 * Constructor
 */
Noun::Noun(Rune* aux, Rune* adj, Rune* noun)
    : Word(std::vector<Rune*> {aux, adj, noun})
{
    if(!aux->isAuxilliary() || !adj->isAdjective() || !noun->isNoun())
        throw;

    _modality = aux->modality();
    _select = adj->selectTargetFromCandidatesFn();
    _findCandidates = noun->getTargetCandidatesFn();

    _targetWrapper = auxAdjNounAcquire;
}

/**
 * Constructor
 */
Noun::Noun(Rune* adj, Rune* noun)
    : Word(std::vector<Rune*> {adj, noun})
{
    if (!adj->isAdjective() || !noun->isNoun())
        throw;
    _select = adj->selectTargetFromCandidatesFn();
    _findCandidates = noun->getTargetCandidatesFn();

    _targetWrapper = adjNounAcquire;
}
//}

//{Methods
Combatable* Noun::acquireTarget(Mob* caster, SpellContext& field)
{
    SpellData data = SpellData();
    return _targetWrapper(this, caster, field, data);
}

WordType Noun::type(void) const
{
    return WordType::NOUN;
}

std::vector<Combatable*> Noun::acquireCandidates(Mob* caster, SpellContext& field)
{
    SpellData data;
    return _multiTargeter(caster, field, data);
}
//}
