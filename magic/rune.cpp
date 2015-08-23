#include "rune.h"
#include "../res/templates.h"
using namespace Magic;


Rune::Rune(const Templates::RuneTemplate& tmpl)
{
    _name = tmpl.Name;
    _imagePath = tmpl.ImagePath;
    _getTarget = tmpl.GetTarget;
    _getTargetCandidates = tmpl.GetTargetCandidates;
    _selectTargetFromCandidates = tmpl.SelectTargetFromCandidates;
    _performAction = tmpl.PerformAction;
    _metaAction = tmpl.MetaAction;
    _adverbFlag = tmpl.Flag;

}

Rune::~Rune(void) {}

//{Properties
std::string Rune::name(void) const { return _name; }
std::string Rune::imagePath(void) const { return _imagePath; }
Targeter Rune::getTargetFn(void) const { return _getTarget; }
MultiTargeter Rune::getTargetCandidatesFn(void) const { return _getTargetCandidates; }
Selecter Rune::selectTargetFromCandidatesFn(void) const { return _selectTargetFromCandidates; }
Action Rune::performActionFn(void) const { return _performAction; }
MetaAction Rune::metaActionFn(void) const { return _metaAction; }
AdverbFlag Rune::adverbFlag(void) const { return _adverbFlag; }

bool Rune::isProperNoun(void) const
{
    return _getTarget != nullptr;
}

bool Rune::isNoun(void) const
{
    return _selectTargetFromCandidates != nullptr;
}

bool Rune::isAdjective(void) const
{
    return _selectTargetFromCandidates != nullptr;
}

bool Rune::isVerb(void) const
{
    return _performAction != nullptr;
}

bool Rune::isAdverb(void) const
{
    return _metaAction != nullptr;
}

bool Rune::isModifier(void) const
{
    return _adverbFlag != AdverbFlag::NA;
}

//}
    /*
    struct RuneTemplate
    {
        std::string Name;
        std::string ImagePath;
        Magic::Targeter GetTarget;
        Magic::MultiTargeter GetTargetCandidates;
        Magic::Selecter SelectTargetFromCandidates;
        Magic::Action PerformAction;
        Magic::Meta MetaAction;
        int AddEffect;
        int AddCost;
        int AddDuration;

        float ModEffect;
        float ModCost;
        float ModDuration;
    };*/
