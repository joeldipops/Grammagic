#ifndef JOBCLASS_H_INCLUDED
#define JOBCLASS_H_INCLUDED

#include <string>
#include "../res/templates.h"
#include "../persistence/saveable.h"

namespace Play
{
    class JobClass : public Persistence::Saveable
    {
        public:
            JobClass(){};
            JobClass(const Resources::JobTemplate&);
            ~JobClass(void);

            const std::string& name(void) const;
            Persistence::SavedObjectCode code(void) const;

            float staminaGrowthCoefficient(void) const;
            float skillGrowthCoefficient(void) const;
            float speedGrowthCoefficient(void) const;
            float resistanceGrowthCoefficient(void) const;
            float defenceGrowthCoefficient(void) const;

        private:
            std::string _name;
            float _staminaGrowthCoefficient;
            float _skillGrowthCoefficient;
            float _speedGrowthCoefficient;
            float _resistanceGrowthCoefficient;
            float _defenceGrowthCoefficient;
            Persistence::SavedObjectCode _code;
    };
}


#endif
