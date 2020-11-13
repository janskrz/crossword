#pragma once

#include <cstdint>
#include <utility>
#include <vector>
#include <map>
#include <set>
#include <memory>

#include "word.h"

namespace grid
{

enum Direction // DO NOT CHANGE ORDER in_bounds check relies on it
{
    VERTICAL = 0,
    HORIZONTAL = 1
};

typedef std::int_fast32_t gidx;
typedef struct Location
{
    gidx row;
    gidx column;
    grid::Direction direction;
} Location;

class Grid
{
private:
    char const EMPTY_CHAR = '-';

    // size of the internal grid
    gidx m_internal_row_count;
    gidx m_internal_column_count;
    std::unique_ptr<char[]> m_grid;

    // words placed on the grid
    std::vector<std::pair<grid::Location, Word> > m_words;

    std::map<char, std::set<gidx> > m_char_loc_lookup;

    // maximum number of rows/columns that can be used by valid crossword.
    // Note: m_internal_[row/column]_count may be larger to allow for flexibility
    // in adding new words
    gidx m_max_row_count;
    gidx m_max_column_count;

    // bounds used by the words places on the current grid
    gidx m_min_row_used;
    gidx m_max_row_used;
    gidx m_min_column_used;
    gidx m_max_column_used;

public:
    Grid(gidx max_row_count, gidx max_column_count);

    /**
        Checks if the word 'word' can be placed at location 'loc' without running
        out-of-bounds and violating the size constraints of the grid.
     */
    bool in_bounds(Word const &word, grid::Location const &loc) const;

    /**
        Place a word on grid and adds it to the m_words buffer.  Note that no
        validity our out-of-bounds checks are performed! This means that letters
        of crossing words will be overwritten by this placement.
        Thus, it is vital to check beforehand if this words can be placed here
        without destroying the grid.
     */
    void place_word_unchecked(Word const &word, grid::Location const &loc);

    /**
        Place this first word in an empty grid.
        @return true if word was successfully placed. False if word could not be
        placed, e.g. because the grid is not empty.
     */
    bool place_first_word(Word const &word, grid::Direction direction);

    /**
        Prints the current grid on console
     */
    void print() const;
};

} //namespace grid

