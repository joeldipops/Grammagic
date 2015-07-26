#ifndef TARGETALL_H_INCLUDED
#define TARGETALL_H_INCLUDED

#include "../mapObject.h"
#include "../play/combatable.h"

#include <vector>
#include <algorithm>


namespace Magic
{
    class TargetAll : public Combatable
    {
        public:
            TargetAll(std::vector<Combatable*>);
            virtual int changeStamina(int);
            virtual float changeSpeed(float);
            virtual float changeDefence(float);
            virtual float changeResistance(float);
            virtual float changeSkill(float);

            virtual int stamina(void) const;
            virtual float speed(void) const;
            virtual float resistance(void) const;
            virtual float defence(void) const;
            virtual float skill(void) const;

        private:
            float getDistribution(float) const;
            float calculateMean(std::vector<float>) const;
            float calculateMean(std::vector<int>) const;
            std::vector<Combatable*> _all;
    };
}

#endif
