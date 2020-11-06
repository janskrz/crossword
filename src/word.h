#pragma once

#include <vector>
#include <string>

typedef struct Word {
    std::string clue;
    std::string word;
} Word;

typedef std::vector<Word> WordList;
