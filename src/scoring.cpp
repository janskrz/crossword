#include "scoring.h"
#include "simplescoring.h"

using namespace scoring;

std::unique_ptr<Scoring> Scoring::create(std::string const &type, INIReader const &config)
{
    if (type == "simple")
    {
        return std::make_unique<SimpleScoring>(config);
    }
    return nullptr;
}
