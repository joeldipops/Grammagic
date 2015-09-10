#ifndef COMBATABLE_H_INLCUDED
#define COMBATABLE_H_INLCUDED

namespace Play
{
    class Combatable
    {
        public:
            virtual ~Combatable(void) {};
            virtual float changeSkill(float) = 0;
            virtual int changeStamina(int) = 0;
            virtual float changeSpeed(float) = 0;
            virtual float changeDefence(float) = 0;
            virtual float changeResistance(float) = 0;
            virtual unsigned short maxStamina(void) const = 0;
            virtual int stamina(void) const = 0;
            virtual float speed(void) const = 0;
            virtual float resistance(void) const = 0;
            virtual float defence(void) const = 0;
            virtual float skill(void) const = 0;
            virtual bool isInCombat(void) {return _isInCombat; };
            virtual bool isInCombat(bool isInCombat_) {_isInCombat = isInCombat_; return _isInCombat; };
            virtual void endCombat(void) = 0;
        private:
            bool _isInCombat = false;
    };
}

#endif
