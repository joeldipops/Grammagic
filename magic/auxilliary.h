#ifndef AUXILLIARY_H_INCLUDED
#define AUXILLIARY_H_INCLUDED

#include "../globalConstants.h"
#include "word.h"

namespace Magic
{
    class Auxilliary : public Word
    {
        WordType type(void) const;
        Modality modality(void) const;
    };
}

#endif
