#ifndef MAGICTYPEDEFS_H_INCLUDED
#define MAGICTYPEDEFS_H_INCLUDED

#include <vector>

namespace Play
{
    class Mob;
    class BattleField;
    class Combatable;
}


namespace Magic
{
    typedef Combatable* (*Targeter)(Mob*, BattleField*);
    typedef std::vector<Combatable*> (*MultiTargeter) (Mob*, BattleField*);
    typedef Combatable* (*Selecter) (Mob*, BattleField*, std::vector<Combatable*>);
    typedef void (*Action) (Combatable*, Combatable*, int, int);

    /**
    * Casts a spell or carries out some other in-combat command.
    * @param _this The command being carried out.
    * @param battlefield holds various data on combat currently in progress.
    * @return The time before the mob that made the command can act again.
    */
    typedef int (*Effect)(Command* context, Mob*, BattleField*);
}

#endif
