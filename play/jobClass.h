#ifndef JOBCLASS_H_INCLUDED
#define JOBCLASS_H_INCLUDED

#include <string>
#include "../res/templates.h"

namespace Play
{
    class JobClass
    {
        public:
            JobClass(){};
            JobClass(const Templates::JobTemplate&);
            ~JobClass(void);

            const std::string& name(void) const;

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
    };
}


#endif
