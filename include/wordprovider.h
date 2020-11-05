#include <string>

#include "word.h"

class WordProvider {
public:
    /**
        Retrieves a list of crossword words from an abstract source.
        @param wordlist The word list to which the retrieved words are appended
        to.
    */
    virtual void retrieve_word_list(WordList& wordlist) const = 0;
};