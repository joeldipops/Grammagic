#include "modifier.h"
#include <math.h>
Modifier::Modifier(){}

Modifier::Modifier(double multiplier, bool isOrderReversed)
{
    _addition.set(0);
    _multiplier.set(multiplier);
    _isOrderReversed = isOrderReversed;
}

Modifier::Modifier(int addition, bool isOrderReversed)
{
    _addition.set(addition);
    _multiplier.set(1);
    _isOrderReversed = isOrderReversed;
}

Modifier::Modifier(int addition, double multiplier, bool isOrderReversed)
{
    _addition.set(addition);
    _multiplier.set(multiplier);
    _isOrderReversed = isOrderReversed;
}

int Modifier::add(int input) const
{
    return  (_addition.isNull()) ? input : input + _addition.get();
}

int Modifier::multiply(int input) const
{
    return (_multiplier.isNull()) ? input : input * _multiplier.get();
}

int Modifier::modify(int input) const
{
    float result = input;
    if(_isOrderReversed)
    {
        result = (_addition.isNull()) ? result : result + _addition.get();
        result = (_multiplier.isNull()) ? result : result * _multiplier.get();
    }
    else
    {
        result = (_multiplier.isNull()) ? result : result * _multiplier.get();
        result = (_addition.isNull()) ? result : result + _addition.get();
    }

    return int(ceil(result));
}
