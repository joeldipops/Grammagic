#ifndef TARGETALL_H_INCLUDED
#define TARGETALL_H_INCLUDED

#include <vector>
#include <algorithm>

#include "../play/mapObject.h"
#include "../play/combatable.h"

using namespace Play;
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

            virtual unsigned short maxStamina(void) const;
            virtual int stamina(void) const;
            virtual float speed(void) const;
            virtual float resistance(void) const;
            virtual float defence(void) const;
            virtual float skill(void) const;

            void endCombat(void);

        private:
            float getDistribution(float) const;
            float calculateMean(std::vector<float>) const;
            float calculateMean(std::vector<int>) const;
            std::vector<Combatable*> _all;
    };
}

#endif
