#ifndef PARTY_H_INCLUDED
#define PARTY_H_INCLUDED

#include "mapObject.h"
#include "../res/templates.h"
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
            PC* addLeader(const Resources::PCTemplate&);
            PC* addMember(const Resources::PCTemplate&);
            PC* removeMember(PC*);
            PlayStateContainer& onInspect(PlayStateContainer&);
            void buryTheDead(void);
            void endCombat(void);
            void getSpoils(BattleField& field);

            PC* memberAt(natural) const;
            int getIndexOf(const Mob*) const;
            void reorder(int, int);

            bool isDefeated(void) const;
            bool isPlayerParty(void) const;
            int x(int); using MapObject::x;
            int y(int); using MapObject::y;

            const std::vector<Rune*> runeCollection(void) const;
            void addRunes(Rune*);
            void addRunes(std::vector<Rune*>);
            std::vector<Rune*> _runeCollection;


        private:
            std::vector<PC*> _members;
            std::vector<PC*> _bench;
            static Resources::MapObjectTemplate ctrTmpl;

    };
}
#endif
