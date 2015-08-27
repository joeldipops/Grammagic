#ifndef NOUN_H_INCLUDED
#define NOUN_H_INCLUDED

#include <vector>

#include "word.h"
#include "modifier.h"
#include "../mapObject.h"
#include "../play/battleField.h"

namespace Play { class Mob; }
using namespace Play;
namespace Magic
{
    class Noun;
    typedef Combatable* (*NounTargeter)(Noun* context, Mob* caster, BattleField* field, SpellData& data);

    class Noun : public Word
    {
        public:
            Noun(){};
            Noun(Rune* properNoun);
            Noun(Rune* aux, Rune* adj, Rune* noun);
            Noun(Rune* adj, Rune* noun);
            Noun(MultiTargeter, std::string, Modifier, Modifier, Modifier);
            std::vector<Combatable*> acquireCandidates(Mob*, BattleField*);
            WordType type(void) const;
            Combatable* acquireTarget(Mob*, BattleField*);

        private:
            MultiTargeter _multiTargeter;
            MultiTargeter _findCandidates;
            Targeter _target;
            Selecter _select;
            Modality _modality;

            NounTargeter _targetWrapper;

            friend Combatable* properNounAcquire(Noun*, Mob* caster, BattleField* field, SpellData& data);
            friend Combatable* auxAdjNounAcquire(Noun*, Mob* caster, BattleField* field, SpellData& data);
            friend Combatable* adjNounAcquire(Noun*, Mob* caster, BattleField* field, SpellData& data);
    };
    Combatable* properNounAcquire(Noun* context, Mob* caster, BattleField* field, SpellData& data);
    Combatable* auxAdjNounAcquire(Noun*, Mob* caster, BattleField* field, SpellData& data);
    Combatable* adjNounAcquire(Noun*, Mob* caster, BattleField* field, SpellData& data);
}

#endif
