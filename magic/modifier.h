#ifndef MODIFIER_H_INCLUDED
#define MODIFIER_H_INCLUDED

#include <math.h>

#include "../util/utils.h"

using namespace Util;

namespace Magic
{
    class Modifier
    {
        public:
            Modifier();
            Modifier(int, double, bool = false);
            Modifier(int, bool = false);
            Modifier(double, bool = false);
            double modify(double) const;
            double add(double) const;
            double multiply(double) const;

        private:
            Nullable<int> _addition;
            Nullable<double> _multiplier;
            bool _isOrderReversed;
    };
}
#endif
