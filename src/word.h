#pragma once

#include <cstdint>
#include <vector>
#include <string>

typedef std::uint_fast16_t wid;

typedef struct Word
{
    wid const id;

    std::string const clue;
    std::string const word;

    std::int_fast16_t const length;

    Word(wid id_, std::string const &clue_, std::string const &word_) :
        id(id_), clue(clue_), word(word_), length(word_.length())
    {
    }

    char const& operator[](int index) const
    {
        return word[index];
    }
} Word;

typedef std::vector<Word> WordList;
