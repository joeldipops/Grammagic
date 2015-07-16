#include "battleCommands.h"

using namespace Magic;
std::vector<Word*> Commands::allCommands = std::vector<Word*>();

// A mob hostile to the caster.
Noun Commands::ENEMY = Noun(enemies, std::string("ENEMY"), Modifier(1.0), Modifier(1.0), Modifier(1.0));

// The mob with the most or least stamina
Adjective Commands::FRESHEST = Adjective(weakest, std::string("FRESHEST"), Modifier(1.0), Modifier(1.0), Modifier(1.0));
Adjective Commands::SICKEST = Adjective(strongest, std::string("SICKEST"), Modifier(1.0), Modifier(1.0), Modifier(1.0));

// The caster
ProperNoun Commands::SELF = ProperNoun(self, std::string("SELF"), Modifier(.8), Modifier(1.0), Modifier(1.0));

// Transfers health from one to the other.
Verb Commands::WEAKEN = Verb(weaken, std::string("WEAKEN"), Modifier(50), Modifier(10), Modifier(2000), -2, 1, 0);
Verb Commands::HEAL = Verb(strengthen, std::string("HEAL"), Modifier(50), Modifier(10, 1.0), Modifier(2000), -1, 1, 0);

// Trades health for speed.
Verb Commands::HASTEN = Verb(hasten, std::string("HASTEN"), Modifier(5), Modifier(10, 1.0), Modifier(3000), -2, 1, 1);
Verb Commands::SLOW = Verb(slow, std::string("SLOW"), Modifier(5), Modifier(10, 1.0), Modifier(3000), -1, 1, 1);

// Takes more to deal more, takes less to deal less
Adverb Commands::HEAVIER = Adverb(std::string("HEAVIER"), Modifier(1.25), Modifier(1.25), Modifier(1.0));
Adverb Commands::LIGHTER = Adverb(std::string("LIGHTER"), Modifier(.75), Modifier(.75), Modifier(1.0));

// Halves the duration, at a greater cost and some loss of effectiveness
Adverb Commands::FASTER = Adverb(std::string("FASTER"), Modifier(0.9), Modifier(1.5), Modifier(0.5));


