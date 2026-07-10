#ifndef SPELLCHECKER_H
#define SPELLCHECKER_H

#include <string>

class SpellChecker {
public:
    static int calculateLevenshtein(const std::string& s1, const std::string& s2);
};

#endif // SPELLCHECKER_H