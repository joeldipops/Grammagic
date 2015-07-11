#include "battleCommands.h"

using namespace Magic;
std::vector<Word*> Commands::allCommands = std::vector<Word*>();

Noun Commands::ENEMY = Noun(enemies, std::string("ENEMY"), Modifier(1.0), Modifier(1.0), Modifier(1.0));


Adjective Commands::WEAKEST = Adjective(weakest, std::string("WEAKEST"), Modifier(1.0), Modifier(1.0), Modifier(1.0));

Adjective Commands::STRONGEST = Adjective(strongest, std::string("WEAKEST"), Modifier(1.0), Modifier(1.0), Modifier(1.0));


ProperNoun Commands::SELF = ProperNoun(self, std::string("SELF"), Modifier(.8), Modifier(1.0), Modifier(1.0));

Verb Commands::WEAKEN = Verb(weaken, std::string("WEAKEN"), Modifier(50), Modifier(10), Modifier(2000), -2, 1, 0);

Verb Commands::HEAL = Verb(strengthen, std::string("HEAL"), Modifier(50), Modifier(10, 1.0), Modifier(2000), -1, 1, 0);

Adverb Commands::LIGHTER = Adverb(std::string("LIGHTER"), Modifier(.75), Modifier(.75), Modifier(1.0));

// Takes more from the source to deal more to the target.
Adverb Commands::HEAVIER = Adverb(std::string("HEAVIER"), Modifier(1.25), Modifier(1.25), Modifier(1.0));
