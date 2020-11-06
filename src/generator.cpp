#include <iostream>

#include "generator.h"
#include "csvwordprovider.h"

#include "INIReader.h"

using namespace std;

int main() {
    cout << "Starting crossword generator." << endl;

    INIReader reader("test.ini");

    CSVWordProvider provider(string("testlist.csv"), true);
    WordList list;
    provider.retrieve_word_list(list);


    for (Word w : list) {
        cout << w.clue << " " << w.word << endl;
    }

    return 0;
}
