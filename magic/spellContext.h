#ifndef SPELLCONTEXT_H_INCLUDED
#define SPELLCONTEXT_H_INCLUDED

#include <vector>

namespace Play { class Mob; class Combatable; class Enemy; class PC;}

using namespace Play;

namespace Magic
{
    class SpellContext
    {
        public:
            SpellContext(void) {};
            SpellContext(const std::vector<Mob*>&, const std::vector<Mob*>&);
            SpellContext(const std::vector<PC*>&, const std::vector<Enemy*>&);
            ~SpellContext(void);

            std::vector<Mob*>& pcs(void);
            std::vector<Mob*> pcs(void) const;

            std::vector<Mob*>& hostiles(void);
            std::vector<Mob*> hostiles(void) const;

            virtual void addToField(Combatable*, bool isPlayerAllied);

            bool isInCombat(bool);
            bool isInCombat(void) const;

            std::vector<Combatable*> participants(void);

            virtual bool areAllied(const Combatable* one, const Combatable* other) const;

        protected:
            std::vector<Mob*> addPC(Mob*);
            std::vector<Mob*> addHostile(Mob*);

        private:
            std::vector<Mob*> _pcs;
            std::vector<Mob*> _hostiles;

            std::vector<Combatable*> _playerAllied = std::vector<Combatable*>(0);
            std::vector<Combatable*> _nonPlayerAllied = std::vector<Combatable*>(0);
            std::vector<Combatable*> _rubbishBin = std::vector<Combatable*>(0);
            bool _isInCombat = false;

    };
}
#endif
