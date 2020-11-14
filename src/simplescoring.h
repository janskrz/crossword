#pragma once

#include "scoring.h"

#include "INIReader.h"

namespace scoring {

class SimpleScoring : public Scoring
{
private:
    score const m_base_score;
    score const m_placed_word_bonus;
    score const m_placed_letter_bonus;
    score const m_word_crossing_bonus;
    score const m_missing_word_penalty;
    score const m_used_row_penalty;
    score const m_used_column_penalty;

public:
    SimpleScoring(INIReader const &config);

    score score_grid(grid::Grid const &grid,
                     std::int_fast32_t unplaced_word_count) const override;
};

} // namespace score