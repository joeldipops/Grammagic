#ifndef RUNE_H_INCLUDED
#define RUNE_H_INCLUDED

#include <string>
#include "modifier.h"
#include "magicTypedefs.h"
namespace Templates { class RuneTemplate; }

namespace Magic
{
    /*
    class Verb;
    class Noun;
    class Adjective;
    class Adverb;
*/
    class Rune
    {
        public:
            Rune(const Templates::RuneTemplate&);
            ~Rune(void);

            std::string name(void) const;
            std::string imagePath(void) const;
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

            Targeter getTargetFn(void) const;
            MultiTargeter getTargetCandidatesFn(void) const;
            Selecter selectTargetFromCandidatesFn(void) const;
            Action performActionFn(void) const;
            MetaAction metaActionFn(void) const;
            Modality modality(void) const;

        private:
            std::string _name;
            std::string _imagePath;
            Targeter _getTarget = nullptr;
            MultiTargeter _getTargetCandidates = nullptr;
            Selecter _selectTargetFromCandidates = nullptr;
            Action _performAction = nullptr;
            MetaAction _metaAction = nullptr;
            Modality _modality = Modality::NA;

            int _addEffect = 0;
            int _addCost = 0;
            int _addDuration = 0;

            float _modifyEffect = 1;
            float _modifyCost = 1;
            float _modifyDuration = 1;
    };
}

#endif
