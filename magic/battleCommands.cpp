#include "battleCommands.h"

using namespace Magic;
std::vector<Word*> Commands::allCommands = std::vector<Word*>();

// A mob hostile to the caster.
Noun Commands::ENEMY = Noun(enemies, std::string("ENEMY"), Modifier(1.0), Modifier(1.0), Modifier(1.0));

// The mob with the most or least stamina
Adjective Commands::FRESHEST = Adjective(freshest, std::string("FRESHEST"), Modifier(1.0), Modifier(1.0), Modifier(1.0));
Adjective Commands::SICKEST = Adjective(sickest, std::string("SICKEST"), Modifier(1.0), Modifier(1.0), Modifier(1.0));

// The fastest or slowest mob
Adjective Commands::FASTEST = Adjective(fastest, std::string("FASTEST"), Modifier(1.0), Modifier(1.0), Modifier(1.0));
Adjective Commands::SLOWEST = Adjective(slowest, std::string("SLOWEST"), Modifier(1.0), Modifier(1.0), Modifier(1.0));

// The mob with the most or least magical resistance
Adjective Commands::WARDED = Adjective(warded, std::string("WARDED"), Modifier(1.0), Modifier(1.0), Modifier(1.0));
Adjective Commands::EXPOSED = Adjective(exposed, std::string("EXPOSED"), Modifier(1.0), Modifier(1.0), Modifier(1.0));

// The mob with the most or least physical defence
Adjective Commands::GUARDED = Adjective(guarded, std::string("GUARDED"), Modifier(1.0), Modifier(1.0), Modifier(1.0));
Adjective Commands::VULNERABLE = Adjective(vulnerable, std::string("VULNERABLE"), Modifier(1.0), Modifier(1.0), Modifier(1.0));

// The mob with the most or least magical skill
Adjective Commands::STRONGEST = Adjective(strongest, std::string("STRONGEST"), Modifier(1.0), Modifier(1.0), Modifier(1.0));
Adjective Commands::WEAKEST = Adjective(weakest, std::string("WEAKEST"), Modifier(1.0), Modifier(1.0), Modifier(1.0));

// Equally distribute effect over all candidates
Adjective Commands::ALL = Adjective(all, std::string("ALL"), Modifier(1.1), Modifier(1.2), Modifier(1.05));

// The caster
ProperNoun Commands::CASTER = ProperNoun(self, std::string("CASTER"), Modifier(.8), Modifier(1.0), Modifier(1.0));

// Buff an ally's defence or reduce an enemy's
Verb Commands::DEFEND = Verb(defend, std::string("DEFEND"), Modifier(50), Modifier(10), Modifier(2000), -1, 1, 0, true);
Verb Commands::ENDANGER = Verb(endanger, std::string("ENDANGER"), Modifier(50), Modifier(10, 1.0), Modifier(2000), -1, 1, 0);

// Buff an ally's magical resistance or reduce an enemy's
Verb Commands::WARD = Verb(ward, std::string("WARD"), Modifier(50), Modifier(10), Modifier(2000), -1, 1, 0, true);
Verb Commands::EXPOSE = Verb(expose, std::string("EXPOSE"), Modifier(50), Modifier(10, 1.0), Modifier(2000), -1, 1, 0);

// Buff an ally's magical skill or reduce an enemy's
Verb Commands::ENHANCE= Verb(enhance, std::string("ENHANCE"), Modifier(50), Modifier(10), Modifier(2000), -1, 1, 0, true);
Verb Commands::IMPAIR = Verb(impair, std::string("IMPAIR"), Modifier(50), Modifier(10, 1.0), Modifier(2000), -1, 1, 0);

// Transfers health from one to the other.
Verb Commands::HURT = Verb(weaken, std::string("HURT"), Modifier(50), Modifier(10), Modifier(2000), -2, 1, 0);
Verb Commands::HEAL = Verb(strengthen, std::string("HEAL"), Modifier(50), Modifier(10, 1.0), Modifier(2000), -1, 1, 0, true);

// Trades health for speed.
Verb Commands::HASTEN = Verb(hasten, std::string("HASTEN"), Modifier(50), Modifier(10, 1.0), Modifier(3000), -2, 1, 1, true);
Verb Commands::SLOW = Verb(slow, std::string("SLOW"), Modifier(50), Modifier(10, 1.0), Modifier(3000), -1, 1, 1);

// Takes more to deal more, takes less to deal less
Adverb Commands::HEAVIER = Adverb(std::string("HEAVIER"), Modifier(1.25), Modifier(1.25), Modifier(1.0));
Adverb Commands::LIGHTER = Adverb(std::string("LIGHTER"), Modifier(.75), Modifier(.75), Modifier(1.0));

// Halves the duration, at a greater cost and some loss of effectiveness
Adverb Commands::FASTER = Adverb(std::string("FASTER"), Modifier(0.9), Modifier(1.5), Modifier(0.5));
