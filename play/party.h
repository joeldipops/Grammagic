#ifndef PARTY_H_INCLUDED
#define PARTY_H_INCLUDED

#include "../mapObject.h"
#include "pc.h"

namespace Play
{
    class Party : public MapObject
    {
        public:
            Party(void);
            Party(std::vector<PC*>);
            ~Party(void);
            PC* leader(void) const;
            const std::vector<PC*> members(void) const;
            PC* memberAt(unsigned int) const;
            PC* addMember(void);
            PC* addLeader(void);

            void buryTheDead(void);
            void endCombat(void);

            int getIndexOf(const Mob*) const;
            void reorder(int, int);

            bool isDefeated(void) const;
            bool isPlayerParty(void) const;
            int x(int); using MapObject::x;
            int y(int); using MapObject::y;

        private:
            std::vector<PC*> _members;
            std::vector<PC*> _bench;

    };
}
#endif
