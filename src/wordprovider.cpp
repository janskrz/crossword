#include <utility>

#include "csvwordprovider.h"

using namespace std;

map<string, function<unique_ptr<WordProvider>(string)> > WordProvider::m_factories =
{
    { "csv", [](const string& location){
          return make_unique<CSVWordProvider>(location);
      }
    }
};

string WordProvider::trim(string const &str)
{
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

unique_ptr<WordProvider> WordProvider::create(const string& type, const string& location)
{
    if(m_factories.count(type) > 0)
    {
        return m_factories[type](location);
    }
    else
    {
        return nullptr;
    }
}