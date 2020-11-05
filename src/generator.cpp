#include <iostream>

#include "generator.h"
#include "csvwordprovider.h"

using namespace std;

int main() {
    cout << "Starting crossword generator." << endl;

    CSVWordProvider provider(string("testlist.csv"), true);
    WordList list;
    provider.retrieve_word_list(list);


    for (Word w : list) {
        std::cout << w.clue << " " << w.word << endl;
    }

    return 0;
}
