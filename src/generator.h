#pragma once

#include <cstdint>
#include <random>
#include <utility>

#include "wordprovider.h"
#include "scorer.h"
#include "grid.h"

class Generator
{
private:
    std::int_fast32_t PROGRESS_EVER_N_GRIDS = 50;
    
    std::default_random_engine m_rng;

    std::int_fast32_t m_gen_count;
    std::int_fast32_t m_cw_max_width;
    std::int_fast32_t m_cw_max_height;

    WordList word_list;
    std::unique_ptr<scoring::Scorer> m_grid_scorer;

    std::unique_ptr<grid::Grid> generate_single_grid();
public:
    Generator(std::int_fast32_t number_of_crosswords_to_generated,
              std::int_fast32_t crossword_max_width, std::int_fast32_t crossword_max_height,
              std::unique_ptr<WordProvider> provider, std::unique_ptr<scoring::Scorer> grid_scorer);

    std::unique_ptr<grid::Grid> generate();
};