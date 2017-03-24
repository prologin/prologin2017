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
#include "position.hh"

#include <algorithm>
#include <utility>


GameState::GameState(rules::Players_sptr players)
    : rules::GameState()
{
    unsigned pi = 0;
    for (auto& p : players->players)
    {
        if (p->type == rules::PLAYER)
        {
            apprentices_.emplace(std::make_pair(p->id, Apprentice(p, pi)));
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
    int id = apprentices_.at(apprentice_id).get_internal_id();
    assert(workbenches_.at(id)[pos1.ligne][pos1.colonne] == VIDE);
    assert(workbenches_.at(id)[pos2.ligne][pos2.colonne] == VIDE);
    change_workbench_case(pos1, sample_.element1, id);
    change_workbench_case(pos2, sample_.element2, id);
}

void GameState::transmute(position pos, unsigned apprentice_id)
{
    assert(apprentices_.count(apprentice_id) != 0);
    int id = apprentices_.at(apprentice_id).get_internal_id();
    assert(workbenches_.at(id)[pos.ligne][pos.colonne] != VIDE);
    // FIXME
}

void GameState::catalyze(position pos, unsigned apprentice_id,
                         case_type new_type)
{
    assert(apprentices_.count(apprentice_id) != 0);
    int id = apprentices_.at(apprentice_id).get_internal_id();
    assert(workbenches_.at(id)[pos.ligne][pos.colonne] != VIDE);
    assert(new_type != VIDE);
    change_workbench_case(pos, new_type, id);
}

void GameState::give(echantillon sample)
{
    assert(sample.element1 != VIDE);
    assert(sample.element2 != VIDE);
    next_sample_ = sample;
}

bool GameState::is_valid_sample_position(echantillon sample, position pos1,
                                         position pos2,
                                         unsigned apprentice_id) const
{
    assert(apprentices_.count(apprentice_id) != 0);
    int id = apprentices_.at(apprentice_id).get_internal_id();
    const Workbench& workbench = workbenches_[id];
    bool has_elements = std::any_of(
        workbench.begin(), workbench.end(), [sample](const auto& line)
        {
            return std::any_of(
                line.begin(), line.end(), [sample](case_type type)
                {
                    return type == sample.element1 || type == sample.element2;
                });
        });
    return is_valid_sample_position(sample, pos1, pos2, workbench,
                                    has_elements);
}

bool GameState::is_valid_sample_position(echantillon sample, position pos1,
                                         position pos2,
                                         const Workbench& workbench,
                                         bool has_elements) const
{
    if (workbench[pos1.ligne][pos1.colonne] != VIDE)
        return false;
    if (workbench[pos2.ligne][pos2.colonne] != VIDE)
        return false;
    if (!has_elements)
        return true;
    static const position offsets[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (position offset : offsets)
    {
        position p1 = pos1 + offset;
        if (sample.element1 == workbench[p1.ligne][p1.colonne])
            return true;
        position p2 = pos2 + offset;
        if (sample.element2 == workbench[p2.ligne][p2.colonne])
            return true;
    }
    return false;
}

case_type GameState::get_cell_type(position pos, unsigned apprentice_id) const
{
    assert(apprentices_.count(apprentice_id) != 0);
    int id = apprentices_.at(apprentice_id).get_internal_id();
    return workbenches_[id][pos.ligne][pos.colonne];
}

int GameState::get_region_size(position pos, unsigned apprentice_id) const
{
    assert(apprentices_.count(apprentice_id) != 0);
    int id = apprentices_.at(apprentice_id).get_internal_id();
    if (workbenches_[id][pos.ligne][pos.colonne] == VIDE)
        return 0;
    int connected_component = connected_components_[id][pos.ligne][pos.colonne];
    int size = 0;
    for (const auto& line : connected_components_[id])
        for (int cc : line)
            if (cc == connected_component)
                ++size;
    return size;
}

std::vector<position> GameState::get_region(position pos,
                                            unsigned apprentice_id) const
{
    std::vector<position> positions;
    assert(apprentices_.count(apprentice_id) != 0);
    int id = apprentices_.at(apprentice_id).get_internal_id();
    if (workbenches_[id][pos.ligne][pos.colonne] == VIDE)
        return positions;
    int connected_component = connected_components_[id][pos.ligne][pos.colonne];
    for (int l = 0; l < TAILLE_ETABLI; ++l)
        for (int c = 0; c < TAILLE_ETABLI; ++c)
            if (connected_components_[id][l][c] == connected_component)
                positions.emplace_back(position{l, c});
    return positions;
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

void GameState::change_workbench_case(position pos, case_type to,
                                      unsigned internal_apprentice_id)
{
    // FIXME
}
