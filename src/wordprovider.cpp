#include <utility>

#include "csvwordprovider.h"

using namespace std;

map<string, function<unique_ptr<WordProvider>(string)>> WordProvider::m_factories =
    {
       { "csv", [](const string& location){
            return make_unique<CSVWordProvider>(location);
       }} 
    };

unique_ptr<WordProvider> WordProvider::create(const string& type, const string& location)
{
    if (m_factories.count(type) > 0)
    {
        return m_factories[type](location);
    } 
    else
    {
        return nullptr;
    }
}