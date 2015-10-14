#ifndef STRINGS_H_INCLUDED
#define STRINGS_H_INCLUDED

#include <string>
// This will be a json file or something when I can be bothered, but for now it's a static class

namespace Resources
{
    class Strings
    {
        public:
            static const std::string Heading;
            static const std::string Victory;
            static const std::string Stamina;
            static const std::string Start;
            static const std::string Quit;
            static const std::string Continue;
            static const std::string Save;
            static const std::string Magic;
            static const std::string Cast;
            static const std::string SaveComplete;
            static const std::string Party;
            static const std::string HutDescription;
            static const std::string Tutorial;

            // Class Names
            static const std::string GlassCannon;
            static const std::string Tank;
            static const std::string WellSpring;
            static const std::string Wasp;
            static const std::string Guard;
            static const std::string Jack;

            static const std::string AName;
            static const std::string BName;
            static const std::string CName;
    };
}
#endif
