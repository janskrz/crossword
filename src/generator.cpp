#include <iostream>
#include <filesystem>

#include "grid.h"
#include "generator.h"
#include "wordprovider.h"

#include "INIReader.h"

#define CONFIG_FILE "config.ini"

using namespace std;

int main(__attribute__((unused)) int argc, char* argv[])
{
    cout << "Starting crossword generator." << endl;

    filesystem::path exec_path = argv[0];
    filesystem::path config_path = exec_path.parent_path().append(CONFIG_FILE);
    cout << "Reading config file " << config_path << endl;

    INIReader reader(config_path);
    if (reader.ParseError() != 0)
    {
        cerr << "Could not read config file! Does file '" << config_path
             << "' exist and is formatted correctly?" << endl;
        return -1;
    }

    filesystem::path wordlistloc = exec_path.parent_path()
                                   .append(reader.Get("wordlist", "location", "INVALID"));

    auto provider = WordProvider::create("csv", wordlistloc);
    WordList list;
    provider->retrieve_word_list(list);

    for (Word w : list)
    {
        cout << w.clue << " " << w.word << endl;
    }

    grid::Grid grid(15, 15);
    grid.place_first_word(list[2], grid::Direction::VERTICAL);
    grid.print();

    return 0;
}
