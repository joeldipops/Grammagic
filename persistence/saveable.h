#ifndef SAVEABLE_H
#define SAVEABLE_H

namespace Persistence
{
    class Saveable
    {
        public:
            virtual SavedObjectCode code(void) const = 0;
    };
}

#endif
