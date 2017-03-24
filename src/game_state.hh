/*
** This file is part of Prologin2017, a rules library for stechec2.
**
** Copyright (c) 2017 Association Prologin <info@prologin.org>
** Copyright (c) 2017 Sacha Delanoue <sacha.delanoue@prologin.org>
**
** Prologin2017 is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Prologin2017 is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with Prologin2017.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GAME_STATE_HH
#define GAME_STATE_HH

#include "constant.hh"

#include <rules/game-state.hh>
#include <rules/player.hh>

#include <array>
#include <unordered_map>

enum class action_type
{
    PLACE,     ///< API function `placer_echantillon`
    TRANSMUTE, ///< API function `transmuter`
    CATALYSE,  ///< API function `catalyser`
    GIVE,      ///< API function `donner_echantillon`
};

struct action
{
    action_type type;  ///< Action type
    position pos1;     ///< Position for PLACE (1st elem), TRANSMUTE, CATALYSE
    position pos2;     ///< Position for PLACE (2nd elem)
    int apprentice_id; ///< Apprentice ID for CATALYZE
    case_type case1;   ///< Case type for CATALYSE, GIVE (1st elem)
    case_type case2;   ///< Case type for GIVE (2nd elem)
};

/// Information about a player; encapsulate its rules::Player_sptr
class Apprentice
{
public:
    /// Constructor from the rules::Player_sptr to encapsulate
    Apprentice(rules::Player_sptr player);

    /// Increase the amount of created gold (thus the score); can be < 0
    void create_gold(int quantity);

    /// Get this player's score (floor of total collected plasma)
    int get_score() const { return player_->score; }

    /// Get this player's name
    const std::string& get_name() const { return player_->name; }

    /// Set this player's name (for tests)
    void set_name(const std::string& name) const { player_->name = name; }

    /// Get the list of actions taken by this player last turn
    const std::vector<action>& get_actions() const { return actions_; }

    /// Empty the list of actions at the start of a new turn
    void reset_actions() { actions_.clear(); }

    /// Register a new action
    void add_action(action action) { actions_.push_back(action); }

private:
    rules::Player_sptr player_;   ///< Encapsulated stechec implementation
    std::vector<action> actions_; ///< Actions taken during last turn
};

class GameState : public rules::GameState
{
public:
    // FIXME
    // additional parameters? for instance map
    GameState(rules::Players_sptr players);
    rules::GameState* copy() const override;
    ~GameState();

private:
    std::unordered_map<unsigned, Apprentice> apprentices_;
    std::array<unsigned, 2> apprentices_ids_;
};

#endif /* !GAME_STATE_HH */
