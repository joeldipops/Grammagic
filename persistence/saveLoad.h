#ifndef SAVELOAD_H_INCLUDED
#define SAVELOAD_H_INCLUDED

#include <string>

#include "../play/pc.h"
#include "../play/party.h"

namespace Persistence
{
    class SaveLoad
    {
        public:
            SaveLoad(const std::string&);
            void save(const Party& party);
            void load(Party& party) const;

        private:
            std::string _path;
            std::vector<byte> getSpellBytes(const PC& pc) const;
            void pushNumeric(std::vector<byte>&, unsigned short);
    };
}

#endif
