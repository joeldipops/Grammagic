#include "modifier.h"

using namespace Magic;

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

double Modifier::add(double input) const
{
    return  (_addition.isNull()) ? input : input + _addition.get();
}

double Modifier::multiply(double input) const
{
    return (_multiplier.isNull()) ? input : input * _multiplier.get();
}

double Modifier::modify(double input) const
{
    double result = input;
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

    return result;
}
