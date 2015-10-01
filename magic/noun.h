#ifndef NOUN_H_INCLUDED
#define NOUN_H_INCLUDED

#include <vector>

#include "word.h"
#include "modifier.h"
#include "../play/mapObject.h"
#include "../play/battleField.h"

namespace Play { class Mob; }
using namespace Play;
namespace Magic
{
    class Noun;
    typedef Combatable* (*NounTargeter)(Noun* context, Mob* caster, SpellContext& field, SpellData& data);

    class Noun : public Word
    {
        public:
            Noun(){};
            Noun(Rune* properNoun);
            Noun(Rune* aux, Rune* adj, Rune* noun);
            Noun(Rune* adj, Rune* noun);
            std::vector<Combatable*> acquireCandidates(Mob*, SpellContext&);
            WordType type(void) const;
            Combatable* acquireTarget(Mob*, SpellContext&);

        private:
            MultiTargeter _multiTargeter;

            MultiTargeter _findCandidates;
            Targeter _target;
            Selecter _select;
            Modality _modality;

            NounTargeter _targetWrapper;

            friend Combatable* properNounAcquire(Noun*, Mob* caster, SpellContext& field, SpellData& data);
            friend Combatable* auxAdjNounAcquire(Noun*, Mob* caster, SpellContext& field, SpellData& data);
            friend Combatable* adjNounAcquire(Noun*, Mob* caster, SpellContext& field, SpellData& data);
    };
    Combatable* properNounAcquire(Noun* context, Mob* caster, SpellContext& field, SpellData& data);
    Combatable* auxAdjNounAcquire(Noun*, Mob* caster, SpellContext& field, SpellData& data);
    Combatable* adjNounAcquire(Noun*, Mob* caster, SpellContext& field, SpellData& data);
}

#endif
