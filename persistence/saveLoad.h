#ifndef SAVELOAD_H_INCLUDED
#define SAVELOAD_H_INCLUDED

#include "../play/pc.h"
#include <string>

namespace Persistence
{
    enum SavedObjectCode
    {
        NewSpell,
        PCPosition,
        HealRune,
        EnemyRune,
        CasterRune,
        HurtRune,
        HastenRune,
        SlowRune,
        HeavierRune,
        LighterRune,
        FreshestRune,
        SickestRune,
        FastestRune,
        SlowestRune,
        FasterRune,
        GuardedRune,
        ExposeRune,
        DefendRune,
        WardRune,
        WardedRune,
        VulnerableRune,
        ExposedRune,
        EndangerRune,
        AllRune,
        ImpairRune,
        EnhanceRune,
        StrongestRune,
        WeakestRune
    };

    class SaveLoad
    {
        public:
            SaveLoad(std::string);
            void save(const PC& pc);
            void load(PC& pc) const;

        private:
            std::string _path;
    };
}

#endif
