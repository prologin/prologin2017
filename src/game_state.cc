/*
** This file is part of Prologin2017, a rules library for stechec2.
**
** Copyright (c) 2017 Association Prologin <info@prologin.org>
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

#include "game_state.hh"

GameState::GameState(rules::Players_sptr players)
    : rules::GameState()
{
    unsigned pi = 0;
    for (auto& p : players->players)
    {
        if (p->type == rules::PLAYER)
        {
            apprentices_.insert({p->id, p});
            apprentices_ids_[pi++] = p->id;
        }
    }

    for (int player = 0; player < 2; ++player)
    {
        for (int i = 0; i < TAILLE_ETABLI; ++i)
        {
            workbenches_[player][i].fill(VIDE);
            connected_components_[player][i].fill(0);
        }
    }

    turn_ = 0;
    next_sample_ = default_sample;
    reset_turn_state();
}

rules::GameState* GameState::copy() const
{
    return new GameState(*this);
}

void GameState::reset_turn_state()
{
    catalysts_ = 0;
    sample_placed_ = false;
    sample_ = next_sample_;
}

int GameState::get_score(unsigned apprentice_id) const
{
    assert(apprentices_.count(apprentice_id) != 0);
    return apprentices_.at(apprentice_id).get_score();
}

void GameState::place_sample(position pos1, position pos2,
                             unsigned apprentice_id)
{
    assert(!sample_placed_);
    assert(apprentices_.count(apprentice_id) != 0);
    // assert(workbenches_.at(id)[pos1.ligne][pos1.colonne] == VIDE);
    // assert(workbenches_.at(id)[pos2.ligne][pos2.colonne] == VIDE);
    // FIXME
}

void GameState::transmute(position pos, unsigned apprentice_id)
{
    assert(apprentices_.count(apprentice_id) != 0);
    // FIXME
}

void GameState::catalyze(position pos, unsigned apprentice_id,
                         case_type new_type)
{
    assert(apprentices_.count(apprentice_id) != 0);
    // FIXME
}

void GameState::give(echantillon sample)
{
    assert(sample.element1 != VIDE);
    assert(sample.element2 != VIDE);
    next_sample_ = sample;
}

case_type GameState::get_cell_type(position pos, unsigned apprentice_id) const
{
    assert(apprentices_.count(apprentice_id) != 0);
    // return workbenches_[id][pos.ligne][pos.colonne];
    // FIXME
}

int GameState::get_region_size(position pos, unsigned apprentice_id) const
{
    assert(apprentices_.count(apprentice_id) != 0);
    // FIXME
}

std::vector<position> GameState::get_region(position p,
                                            unsigned apprentice_id) const
{
    assert(apprentices_.count(apprentice_id) != 0);
    // FIXME
}

std::vector<position_echantillon>
GameState::possible_sample_positions(echantillon sample,
                                     unsigned apprentice_id) const
{
    assert(apprentices_.count(apprentice_id) != 0);
    // FIXME
}

void GameState::hist_add_place(position pos1, position pos2,
                               unsigned apprentice_id)
{
    assert(apprentices_.count(apprentice_id) != 0);
    action a;
    a.type = action_type::PLACE;
    a.pos1 = pos1;
    a.pos2 = pos2;
    apprentices_.at(apprentice_id).add_action(a);
}

void GameState::hist_add_transmute(position pos, unsigned apprentice_id)
{
    assert(apprentices_.count(apprentice_id) != 0);
    action a;
    a.type = action_type::TRANSMUTE;
    a.pos1 = pos;
    apprentices_.at(apprentice_id).add_action(a);
}

void GameState::hist_add_catalize(position pos, unsigned target_apprentice_id,
                                  case_type new_type, unsigned apprentice_id)
{
    assert(apprentices_.count(apprentice_id) != 0);
    action a;
    a.type = action_type::CATALYSE;
    a.pos1 = pos;
    a.apprentice_id = target_apprentice_id;
    a.case1 = new_type;
    apprentices_.at(apprentice_id).add_action(a);
}

void GameState::hist_add_give(echantillon sample, unsigned apprentice_id)
{
    assert(apprentices_.count(apprentice_id) != 0);
    action a;
    a.type = action_type::GIVE;
    a.case1 = sample.element1;
    a.case2 = sample.element2;
    apprentices_.at(apprentice_id).add_action(a);
}

void GameState::reset_history(unsigned apprentice_id)
{
    assert(apprentices_.count(apprentice_id) != 0);
    apprentices_.at(apprentice_id).reset_actions();
}

const std::vector<action>& GameState::get_history(unsigned apprentice_id) const
{
    assert(apprentices_.count(apprentice_id) != 0);
    return apprentices_.at(apprentice_id).get_actions();
}
