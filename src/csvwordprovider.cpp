#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "csvwordprovider.h"

using namespace std;

CSVWordProvider::CSVWordProvider(const string& csv_location,
                                 bool ignore_header, char delim) :
    m_csv_location(csv_location), m_ignore_header(ignore_header), m_delim(delim)
{
}

void CSVWordProvider::retrieve_word_list(WordList& words) const
{
    std::ifstream csv_file(m_csv_location);
    if (!csv_file.is_open())
    {
        throw runtime_error(
                  "Could not open the CSV file! Does it exist?\n"
                  "(Filename: " + m_csv_location + ")"
                  );
    }

    string line;
    if (m_ignore_header)
    {
        std::getline(csv_file, line);
    }

    while (std::getline(csv_file, line))
    {
        stringstream tokens(line);
        Word new_word;
        getline(tokens, new_word.clue, m_delim);
        getline(tokens, new_word.word, m_delim);

        if (new_word.clue.empty() || new_word.word.empty())
        {
            throw runtime_error(
                      "Invalid CSV line format! \n"
                      "Clue or solution word is empty \n"
                      "The offending line: " + line
                      );
        }

        words.push_back(new_word);
        if (!tokens.eof())
        {
            throw runtime_error(
                      "Invalid CSV line format! \n"
                      "Expected two columns separated by '" + string(1, m_delim) + "' but got more.\n"
                      "The offending line: " + line
                      );
        }
    }
}