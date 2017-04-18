#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>

#include "game_state.hh"

namespace
{
const std::string color_reset = "\033[0m";
const std::string color_bold = "\033[1m";

std::string fgcolor(unsigned c)
{
    return "\033[3" + std::to_string(c) + "m";
}

std::string bgcolor(unsigned c)
{
    return "\033[4" + std::to_string(c) + "m";
}
}

void print_workbenches(const GameState* game_state, std::ostream& stream)
{
    const auto& apprentices = game_state->get_apprentices();
    const int current_player_id = game_state->current_player();
    const Apprentice& current_player = apprentices.at(current_player_id);
    const int opponent_id = game_state->opponent(current_player_id);
    const Apprentice& opponent = apprentices.at(opponent_id);
    const echantillon current_sample = game_state->current_sample();

    // First line
    stream << bgcolor(7) << fgcolor(0) << "moi";
    std::fill_n(std::ostream_iterator<char>(stream), TAILLE_ETABLI - 5, ' ');
    stream << std::setw(4) << current_player.get_score()
           << color_reset << color_bold << "     "
           << color_reset << bgcolor(7) << fgcolor(0) << "adv";
    std::fill_n(std::ostream_iterator<char>(stream), TAILLE_ETABLI - 5, ' ');
    stream << std::setw(4) << opponent.get_score() << color_reset << "\n";

    // Second line
    stream << bgcolor(7) << fgcolor(0) << " ";
    for (size_t x = 0; x < TAILLE_ETABLI; ++x)
        stream << x;
    stream << " " << color_reset << color_bold << "Tour:";
    stream << color_reset << bgcolor(7) << fgcolor(0) << " ";
    for (size_t x = 0; x < TAILLE_ETABLI; ++x)
        stream << x;
    stream << " " << color_reset << "\n";

    for (uint8_t y = 0; y < TAILLE_ETABLI; ++y)
    {
        // My workbench
        stream << bgcolor(7) << fgcolor(0) << (unsigned)y;
        for (uint8_t x = 0; x < TAILLE_ETABLI; ++x)
            stream << bgcolor(game_state->get_cell_type({y, x}, current_player_id)) << " ";
        stream << bgcolor(7) << " " << color_reset;

        // Between the two workbenches
        if (y == 0)
            stream << std::setw(4) << game_state->get_turn() << " ";
        else if (y == 1)
            stream << color_bold << "Reçu:" << color_reset;
        else if (y == 2)
        {
            stream << "  "
                   << bgcolor(current_sample.element1) << " "
                   << bgcolor(current_sample.element2) << " "
                   << color_reset << " ";
        }
        else
            stream << "     ";

        // Opponent's workbench
        stream << bgcolor(7) << fgcolor(0) << (unsigned)y;
        for (uint8_t x = 0; x < TAILLE_ETABLI; ++x)
            stream << bgcolor(game_state->get_cell_type({y, x}, opponent_id)) << " ";
        stream << bgcolor(7) << " " << color_reset << "\n";
    }

    // Last line
    const std::string current_player_name = current_player.get_name().substr(0, TAILLE_ETABLI + 2);
    const std::string opponent_name = opponent.get_name().substr(0, TAILLE_ETABLI + 2);

    stream << bgcolor(7) << fgcolor(0) << std::setw(TAILLE_ETABLI + 2) << current_player_name;
    stream << color_reset << "     ";
    stream << bgcolor(7) << fgcolor(0) << std::setw(TAILLE_ETABLI + 2) << opponent_name;
    stream << color_reset << "\n" << std::endl;
}
