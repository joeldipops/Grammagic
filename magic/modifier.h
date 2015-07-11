#ifndef MODIFIER_H_INCLUDED
#define MODIFIER_H_INCLUDED

#include "../util/utils.h"
namespace Magic {
class Modifier
{
    public:
        Modifier();
        Modifier(int, double, bool = false);
        Modifier(int, bool = false);
        Modifier(double, bool = false);
        int modify(int) const;
        int add(int) const;
        int multiply(int) const;

    private:
        Nullable<int> _addition;
        Nullable<float> _multiplier;
        bool _isOrderReversed;

};
}
#endif
