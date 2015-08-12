#ifndef SAVELOAD_H_INCLUDED
#define SAVELOAD_H_INCLUDED

#include <string>

#include "../play/pc.h"
#include "../play/party.h"

namespace Persistence
{
    enum SavedObjectCode
    {
        NewSpell,
        PCPosition,
        NewMember,
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
        WeakestRune,
        AllyRune,
        MemberRune
    };

    class SaveLoad
    {
        public:
            SaveLoad(std::string);
            void save(const Party& party);
            void load(Party& party) const;

        private:
            std::string _path;
            std::vector<byte> getSpellBytes(const PC& pc) const;
            void pushNumeric(std::vector<byte>&, unsigned short);
    };
}

#endif
