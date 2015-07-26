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
            PC* addMember(void);
            PC* addLeader(void);
            bool isDefeated(void) const;
            bool isPlayerParty(void) const;
            int x(int);
            int y(int);
            using MapObject::x;
            using MapObject::y;


        private:
            std::vector<PC*> _members;
    };
}
#endif
