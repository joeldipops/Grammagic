#ifndef RUNE_H_INCLUDED
#define RUNE_H_INCLUDED

namespace Templates { RuneTemplate; }

namespace Magic
{
    class Verb;
    class Noun;
    class Adjective;
    class Adverb;

    class Rune
    {
        public:
            Rune(Templates::RuneTemplate);
            ~Rune(void);

            std::string name(void) const;
            std::string imagePath(void) const;
            Modifier effect(void) const;
            Modifier cost(void) const;
            Modifier duration(void) const;

            bool isAdjective(void) const;
            bool isNoun(void) const;
            bool isVerb(void) const;
            bool isAdverb(void) const;

            Verb* asVerb(void) const;
            Noun* asNoun(void) const;
            Adjective* asAdjective(void) const;
            Adverb* asAdverb(void) const;
        private:
            bool _isAdjective;
            bool _isNoun;
            bool _isVerb;
            bool _isAdjective;
    }
}

#endif
