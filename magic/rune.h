#ifndef RUNE_H_INCLUDED
#define RUNE_H_INCLUDED

#include <string>
#include "modifier.h"
#include "magicTypedefs.h"
#include "../menuItem.h"
#include "../persistence/saveable.h"
using namespace Core;
namespace Resources { class RuneTemplate; }

namespace Magic
{
    class Rune : public MenuItem, public Persistence::Saveable
    {
        public:
            Rune(const Resources::RuneTemplate&);
            ~Rune(void);

            Persistence::SavedObjectCode code(void) const;
            int addEffect(void) const;
            int addCost(void) const;
            int addDuration(void) const;

            float modEffect(void) const;
            float modCost(void) const;
            float modDuration(void) const;

            bool isAdjective(void) const;
            bool isNoun(void) const;
            bool isVerb(void) const;
            bool isAdverb(void) const;
            bool isProperNoun(void) const;
            bool isAuxilliary(void) const;

            float enemyCostMultiplier(void) const;
            float allyEffectMultiplier(void) const;
            float isSameMultiplier(void) const;
            int actionEffectBonus(void) const;
            int actionCostBonus(void) const;
            int actionDurationBonus(void) const;
            bool isBoon(void) const;

            Targeter getTargetFn(void) const;
            MultiTargeter getTargetCandidatesFn(void) const;
            Selecter selectTargetFromCandidatesFn(void) const;
            Action performActionFn(void) const;
            MetaAction metaActionFn(void) const;
            Modality modality(void) const;

        private:
            std::string _name;
            Targeter _getTarget = nullptr;
            MultiTargeter _getTargetCandidates = nullptr;
            Selecter _selectTargetFromCandidates = nullptr;
            Action _performAction = nullptr;
            MetaAction _metaAction = nullptr;
            Modality _modality = Modality::NA;
            float _isSameMultiplier = 1;
            float _enemyCostMultiplier = 1;
            float _allyEffectMultiplier = 1;
            int _actionEffectBonus = 0;
            int _actionCostBonus = 0;
            int _actionDurationBonus = 0;
            bool _isBoon = false;
            Persistence::SavedObjectCode _code;

            int _addEffect = 0;
            int _addCost = 0;
            int _addDuration = 0;

            float _modifyEffect = 1;
            float _modifyCost = 1;
            float _modifyDuration = 1;
    };
}

#endif
