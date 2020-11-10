#include <algorithm>
#include <cstring>
#include <iostream>
#include <sstream>

#include "grid.h"

#define GIDX(row, col) ((row) * m_internal_column_count + col)

using namespace grid;

Grid::Grid(gidx max_row_count, gidx max_column_count) :
    m_internal_row_count(2 * max_row_count), m_internal_column_count(2 * max_column_count),
    m_max_row_count(max_row_count), m_max_column_count(max_column_count),
    // First word will be placed in the center of the internal grid.
    // This is the passed row/column count, as row/column count is doubled internally 
    // to allow flexible placements of words in all directions
    m_min_row_used(max_row_count), m_max_row_used(max_row_count),
    m_min_column_used(max_column_count), m_max_column_used(max_column_count)
{
    // Internally, we keep a grid of double given the column and row size.
    // That way, we can simply place the first word in the middle of the grid
    // and still have space in all directions. Thus, we do not restrict possible
    // solutions due to unfortunate placements of the first word.
    gidx gridsize = max_row_count * 2 * max_column_count * 2;
    m_grid = std::make_unique<char[]>(gridsize);
    std::fill(m_grid.get(), m_grid.get() + gridsize, EMPTY_CHAR);
}

bool Grid::in_bounds(Word const &word, Location const &loc) const
{
    gidx start_row = loc.row;
    gidx start_col = loc.column;
    // Hack to avoid branching. Assumes that vertical = 0, horizontal = 1 
    gidx end_row = loc.row + (word.word.length() - 1) * (1 - loc.direction);
    gidx end_col = loc.column + (word.word.length() - 1) * loc.direction;

    bool out_of_bounds = end_row >= m_internal_row_count;
    out_of_bounds |= start_col >= m_internal_column_count;
    out_of_bounds |= std::max(end_col, m_max_column_used) - std::min(start_col, m_min_column_used)
                     >= m_max_column_count;
    out_of_bounds |= std::max(end_row, m_max_row_used) - std::min(start_row, m_min_row_used)
                     >= m_max_row_count;

    return !out_of_bounds;
}

void Grid::place_word_unchecked(Word const &word, Location const &loc)
{
    switch (loc.direction)
    {
    case Direction::HORIZONTAL:
        std::memcpy(m_grid.get() + GIDX(loc.row, loc.column), word.word.c_str(), word.word.length());
        m_max_row_used = std::max(m_max_row_used, loc.row);
        m_max_column_used = std::max(m_max_column_used, loc.column + word.word.length() - 1);
        break;
    case Direction::VERTICAL:
        for (size_t i = 0; i < word.word.length(); i++)
        {
            m_grid[GIDX(loc.row + i, loc.column)] = word.word[i];
        }
        m_max_row_used = std::max(m_max_row_used, loc.row + word.word.length() - 1);
        m_max_column_used = std::max(m_max_column_used, loc.column);
        break;
    }
    m_min_column_used = std::min(m_min_column_used, loc.column);
    m_min_row_used = std::min(m_min_row_used, loc.row);

    std::pair<Location, Word> placement(loc, word);
    m_words.push_back(placement);
}

bool Grid::place_first_word(Word const &word, Direction direction)
{
    if (!m_words.empty())
    {
        return false;
    }

    // calculate the start position of the word, so that its in the middle of the grid
    Location loc = { m_internal_row_count / 2,  m_internal_column_count / 2, direction };
    switch (direction)
    {
    case Direction::HORIZONTAL:
        loc.column -= word.word.length() / 2;
        break;
    case Direction::VERTICAL:
        loc.row -= word.word.length() / 2;
        break;
    }

    if (!in_bounds(word, loc))
    {
        return false;
    }
    place_word_unchecked(word, loc);
    return true;
}

void Grid::print() const
{
    std::ostringstream os;
    os << "Printing crossword grid" << std::endl << std::endl;
    for (size_t row = 0; row < m_internal_row_count; row++)
    {
        for (size_t column = 0; column < m_internal_column_count; column++)
        {
            os << m_grid[GIDX(row, column)];
        }
        os << std::endl;
    }
    std::cout << os.str() << std::endl;
}
