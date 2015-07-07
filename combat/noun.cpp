#include "noun.h"
Noun::Noun(MapObject* (*target)(Mob*, BattleField*), std::string name, Modifier effect, Modifier cost, Modifier duration)
    : Word(name, effect, cost, duration)
{
    _target = target;
}


// Will create my own battlefield object.  The map is not sufficient for more than a few targets.
// This may also return a group of mobs later...
/**
 * Selects the target from a mob, item or terrain on the battlefield according to the _target function.
 */
MapObject* Noun::aquireTarget(Mob* caster, BattleField* battleField)
{
    return _target(caster, battleField);
}
