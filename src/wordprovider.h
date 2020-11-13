#pragma once

#include <string>
#include <memory>
#include <map>
#include <functional>

#include "word.h"

class WordProvider
{
private:
    static std::map<std::string, std::function<std::unique_ptr<WordProvider>(std::string) > > m_factories;

public:
    /**
       Retrieves a list of crossword words from an abstract source.
       @param wordlist The word list to which the retrieved words are appended
       to.
     */
    virtual void retrieve_word_list(WordList& wordlist) const = 0;

    /**
       Creates and returns a WordProvider of type type.
       @param type Provider type to create
       @param location Path were the file is located
     */
    static std::unique_ptr<WordProvider> create(const std::string& type, const std::string& location);
};