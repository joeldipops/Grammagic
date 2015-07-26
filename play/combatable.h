#ifndef COMBATABLE_H_INLCUDED
#define COMBATABLE_H_INLCUDED

class Combatable
{
    public:
        virtual ~Combatable(void) {};
        virtual float changeSkill(float) = 0;
        virtual int changeStamina(int) = 0;
        virtual float changeSpeed(float) = 0;
        virtual float changeDefence(float) = 0;
        virtual float changeResistance(float) = 0;
        virtual int stamina(void) const = 0;
        virtual float speed(void) const = 0;
        virtual float resistance(void) const = 0;
        virtual float defence(void) const = 0;
        virtual float skill(void) const = 0;
        virtual void endCombat(void) { return; };
};

#endif
