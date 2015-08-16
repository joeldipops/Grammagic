#include "adjective.h"

using namespace Magic;

/**
 * Constructor.
 */
Adjective::Adjective(Selecter selecter, std::string n, Modifier e, Modifier c, Modifier d)
    : Word(n, e, c, d)
{
    _selecter = selecter;
}

/**
 * Type indicating how this word functions in the spell.
 */
WordType Adjective::type(void) const
{
    return WordType::ADJECTIVE;
}

/**
 * Choose a single spell target from a filtered list of candidates
 */
Combatable* Adjective::selectTarget(Mob* caster, BattleField* field, std::vector<Combatable*> candidates)
{
    return _selecter(caster, field, candidates);
}
//-------------------------- Dummy Adjective -----------

/**
 * Constructor
 */
DummyAdjective::DummyAdjective(Selecter s, std::string n, Modifier e, Modifier c, Modifier d)
    : Adjective(s, n, e, c, d)
{}

/**
 * Returns true if this word should not be considered as an actual rune.  It just contains application logic.
 */
bool DummyAdjective::isDummy(void) const
{
    return true;
}
