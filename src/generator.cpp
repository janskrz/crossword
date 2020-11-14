#include <algorithm>
#include <chrono>
#include <iostream>
#include <filesystem>
#include <string>

#include "generator.h"

#include "INIReader.h"

#define SEED_RNG (time(NULL)) // seed is current time in seconds

#define CONFIG_FILE "config.ini"

Generator::Generator(std::int_fast32_t number_of_crosswords_to_generate,
                     std::int_fast32_t crossword_max_width, std::int_fast32_t crossword_max_height,
                     std::unique_ptr<WordProvider> provider, std::unique_ptr<scoring::Scorer> grid_scorer) :
    m_rng(std::default_random_engine{}), m_gen_count(number_of_crosswords_to_generate),
    m_cw_max_width(crossword_max_width), m_cw_max_height(crossword_max_height),
    m_grid_scorer(std::move(grid_scorer))
{
    auto rng_seed = SEED_RNG;
    m_rng.seed(rng_seed);
    provider->retrieve_word_list(word_list);
    std::cout << "Initialized crossword generator. " << std::endl;
    std::cout << "Random generator seed is: " << rng_seed << std::endl;
}

std::unique_ptr<grid::Grid> Generator::generate_single_grid()
{
    std::uniform_int_distribution<int> dist(0, 1); // for generating random vert/horizontal
    auto grid = std::make_unique<grid::Grid>(m_cw_max_height, m_cw_max_width);
    WordList unused_words(word_list);

    // place random first word
    std::shuffle(std::begin(unused_words), std::end(unused_words), m_rng);
    Word const first_word = unused_words.back();
    grid::Direction const first_dir = static_cast<grid::Direction>(dist(m_rng));

    grid->place_first_word(first_word, first_dir);
    unused_words.pop_back();

    // in every iteration, shuffle not yet placed words and try to add them at
    // random valid location in shuffle order. Repeat until no words are left
    // or no remaining word can be placed.
    while (unused_words.size() != 0)
    {
        bool word_placed = false;
        std::shuffle(std::begin(unused_words), std::end(unused_words), m_rng);

        WordList unplaced_words;
        for (auto const &word : unused_words)
        {
            std::vector<grid::Location> valid_placements;
            grid->get_valid_placements(word, valid_placements);
            if (valid_placements.size() == 0)
            {
                unplaced_words.push_back(word);
            }
            else
            {
                std::uniform_int_distribution<int> rand(0, valid_placements.size() - 1);
                grid::Location rand_loc = valid_placements[dist(m_rng)];
                grid->place_word_unchecked(word, rand_loc);
                word_placed = true;
            }
        }
        unused_words = unplaced_words; // TODO use pointers to avoid copies?

        if (!word_placed)
            break;
    }

    return grid;
}

std::unique_ptr<grid::Grid> Generator::generate()
{
    std::cout << "Generating " << m_gen_count << " grids and choosing the best" << std::endl;
    auto begin = std::chrono::high_resolution_clock::now();

    std::unique_ptr<grid::Grid> best_grid = nullptr;
    scoring::score highest_grid_score = 0;
    for (auto i = 1; i <= m_gen_count; i++)
    {
        auto grid = generate_single_grid();
        std::int_fast32_t unplaced_words = word_list.size() - grid->get_placed_word_count();
        scoring::score grid_score = m_grid_scorer->score_grid(*grid, unplaced_words);

        if (best_grid == nullptr || grid_score > highest_grid_score)
        {
            highest_grid_score = grid_score;
            best_grid = std::move(grid);
        }

        if (i % PROGRESS_EVER_N_GRIDS == 0)
        {
            std::cout << "Generated " << i << " out of " << m_gen_count << " grids. " << std::endl;
            std::cout << "The current best grid has a score of "
                      << highest_grid_score << ". It is: " << std::endl;
            best_grid->print_on_console();
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = end - begin;
    auto dur_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    std::cout << "Generated all " << m_gen_count << " grids!" << std::endl;
    std::cout << "This took me a total of " << dur_in_ms / 1000.0 << " seconds." << std::endl;
    std::cout << "The final grid has a score of "
              << highest_grid_score << ". It is: " << std::endl;
    best_grid->print_on_console();

    return best_grid;
}


int main(__attribute__((unused)) int argc, char* argv[])
{
    std::cout << "Starting crossword generator." << std::endl;

    std::filesystem::path exec_path = argv[0];
    std::filesystem::path config_path = exec_path.parent_path().append(CONFIG_FILE);
    std::cout << "Reading config file " << config_path << std::endl;

    INIReader reader(config_path);
    if (reader.ParseError() != 0)
    {
        std::cerr << "Could not read config file! Does file '" << config_path
                  << "' exist and is formatted correctly?" << std::endl;
        return -1;
    }

    std::filesystem::path wordlistloc = exec_path.parent_path()
                                        .append(reader.Get("wordlist", "location", "INVALID"));

    auto cw_gen_count = reader.GetInteger("constraints", "crossword_generation_count", -1);
    auto cw_max_height = reader.GetInteger("constraints", "max_height", -1);
    auto cw_max_width = reader.GetInteger("constraints", "max_width", -1);

    if (cw_gen_count < 0 || cw_max_height < 0 || cw_max_width < 0)
    {
        std::cerr << "Error reading crossword constraints from config!" << std::endl;
        return -1;
    }

    std::string const wordprovider_type = reader.Get("wordlist", "type", "INVALID");
    std::string const scorer_type = reader.Get("scoring", "type", "INVALID");

    auto wordprovider = WordProvider::create(wordprovider_type, wordlistloc);
    auto scorer = scoring::Scorer::create(scorer_type, reader);

    if (wordprovider == nullptr)
    {
        std::cerr << "Error: Could not create word provider of type '"
                  << wordprovider_type << "'" << std::endl;
        return -1;
    }

    if (scorer == nullptr)
    {
        std::cerr << "Error: Could not create scorer of type '"
                  << scorer_type << "'" << std::endl;
        return -1;
    }

    Generator generator(cw_gen_count, cw_max_height, cw_max_width,
                        std::move(wordprovider), std::move(scorer));

    auto grid = generator.generate();

    /*WordList list;
       provider->retrieve_word_list(list);

       for (Word w : list)
       {
        cout << w.clue << " " << w.word << endl;
       }

       auto begin = chrono::high_resolution_clock::now();
       grid::Grid grid(60, 60);
       grid.place_first_word(list[0], grid::Direction::HORIZONTAL);

       for (size_t i = 1; i < list.size(); i++)
       {
        std::vector<grid::Location> buffer;
        grid.get_valid_placements(list[i], buffer);
        if (buffer.size() > 0)
        {
            grid.place_word_unchecked(list[i], buffer[0]);
        }
       }
       auto end = chrono::high_resolution_clock::now();

       grid.print_on_console();
       auto dur = end - begin;
       auto ms = std::chrono::duration_cast<std::chrono::microseconds>(dur).count();
       std::cout << "Generating the grid took " << ms / 1000.0 << "ms" << std::endl;
     */
    return 0;
}
