#pragma once

#include <fstream>

#include "grid.h"

class LatexGenerator
{
private:
    void add_preamble(std::ofstream &) const;
    void add_puzzle_macros(std::ofstream  &) const;
    void add_puzzle(grid::Grid const * const grid, std::ofstream &) const;
    void add_closing(std::ofstream &) const;

public:
    void generate(grid::Grid const * const grid, std::string const &fileloc) const;
};
