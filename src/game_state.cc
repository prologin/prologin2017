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

#include "game_state.hh"
#include "position.hh"

#include <algorithm>
#include <queue>
#include <utility>

static constexpr int CONNECTED_COMPONENT_OF_EMPTY = -1;
static const position offsets[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

Apprentice::Apprentice(rules::Player_sptr player, int internal_id)
    : player_(std::move(player))
    , internal_id_(internal_id)
{
    assert(player_);
    player_->score = 0;
}

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
            connected_components_[player][i].fill(CONNECTED_COMPONENT_OF_EMPTY);
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

int GameState::transmute_gold_scoreval(int area) {
    if (area <= 0)
        return -3;
    return (area - 1) * (area - 1);
}

int GameState::transmute_catalyst_scoreval(int area) {
    if (area <= 0)
        return -3;
    return area - 1;
}

int GameState::transmute_catalyst_outcome(int area) {
    return area / 2;
}

bool GameState::valid_player(unsigned apprentice_id) const {
    return apprentices_.count(apprentice_id) != 0;
}

void GameState::reset_turn_state()
{
    catalysts_ = 0;
    sample_placed_ = false;
    sample_given_ = false;
    sample_ = next_sample_;
}

int GameState::get_score(unsigned apprentice_id) const
{
    assert(apprentices_.count(apprentice_id) != 0);
    return apprentices_.at(apprentice_id).get_score();
}

void GameState::increase_score(unsigned apprentice_id, int by)
{
    assert(apprentices_.count(apprentice_id) != 0);
    apprentices_.at(apprentice_id).increase_score(by);
}

void GameState::place_sample(position pos1, position pos2,
                             unsigned apprentice_id)
{
    assert(!sample_placed_);
    assert(apprentices_.count(apprentice_id) != 0);
    int id = apprentices_.at(apprentice_id).get_internal_id();
    assert(workbenches_.at(id)[pos1.ligne][pos1.colonne] == VIDE);
    assert(workbenches_.at(id)[pos2.ligne][pos2.colonne] == VIDE);
    assert(is_valid_sample_position(sample_, pos1, pos2, apprentice_id));
    change_workbench_case(pos1, sample_.element1, id);
    change_workbench_case(pos2, sample_.element2, id);
    sample_placed_ = true;
}

int GameState::remove_region(position pos, unsigned apprentice_id)
{
    assert(apprentices_.count(apprentice_id) != 0);
    int id = apprentices_.at(apprentice_id).get_internal_id();
    assert(workbenches_.at(id)[pos.ligne][pos.colonne] != VIDE);

    int connected_component = connected_components_[id][pos.ligne][pos.colonne];
    int size = 0;
    for (int l = 0; l < TAILLE_ETABLI; ++l)
    {
        for (int c = 0; c < TAILLE_ETABLI; ++c)
        {
            int& cc = connected_components_[id][l][c];
            if (cc == connected_component)
            {
                ++size;
                cc = CONNECTED_COMPONENT_OF_EMPTY;
                workbenches_[id][l][c] = VIDE;
            }
        }
    }
    return size;
}

void GameState::change_case(position pos, unsigned apprentice_id,
                            case_type new_type)
{
    assert(apprentices_.count(apprentice_id) != 0);
    int id = apprentices_.at(apprentice_id).get_internal_id();
    assert(workbenches_.at(id)[pos.ligne][pos.colonne] != VIDE);
    assert(new_type != VIDE);
    change_workbench_case(pos, new_type, id);
}

void GameState::set_next_sample(echantillon sample)
{
    assert(sample.element1 != VIDE);
    assert(sample.element2 != VIDE);
    next_sample_ = sample;
    sample_given_ = true;
}

bool GameState::is_valid_sample_position(echantillon sample, position pos1,
                                         position pos2,
                                         unsigned apprentice_id) const
{
    assert(apprentices_.count(apprentice_id) != 0);
    int id = apprentices_.at(apprentice_id).get_internal_id();
    const Workbench& workbench = workbenches_[id];
    return is_valid_sample_position(sample, pos1, pos2, workbench,
                                    has_elements(sample, workbench));
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

bool GameState::has_elements(echantillon sample,
                             const Workbench& workbench) const
{
    return std::any_of(
        workbench.begin(), workbench.end(), [sample](const auto& line)
        {
            return std::any_of(
                line.begin(), line.end(), [sample](case_type type)
                {
                    return type == sample.element1 || type == sample.element2;
                });
        });
}

case_type GameState::get_cell_type(position pos, unsigned apprentice_id) const
{
    assert(apprentices_.count(apprentice_id) != 0);
    int id = apprentices_.at(apprentice_id).get_internal_id();
    return workbenches_[id][pos.ligne][pos.colonne];
}

element_propriete GameState::get_element_property(case_type element) {
    switch(element) {
        case PLOMB:
        case FER:
        case CUIVRE:
            return TRANSMUTABLE_OR;

        case SOUFRE:
        case MERCURE:
            return TRANSMUTABLE_CATALYSEUR;

        default:
            return AUCUNE;
    }
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
    int id = apprentices_.at(apprentice_id).get_internal_id();
    const Workbench& workbench = workbenches_[id];
    bool has_elems = has_elements(sample, workbench);

    std::vector<position_echantillon> positions;
    auto try_pos = [&](position p1, position p2)
    {
        if (is_valid_sample_position(sample, p1, p2, workbench, has_elems))
            positions.emplace_back(position_echantillon{p1, p2});
    };
    for (int line = 0; line < TAILLE_ETABLI; ++line)
    {
        for (int column = 0; column < TAILLE_ETABLI; ++column)
        {
            position pos1{line, column};
            if (line + 1 < TAILLE_ETABLI)
            {
                position pos2{line + 1, column};
                try_pos(pos1, pos2);
                try_pos(pos2, pos1);
            }
            if (column + 1 < TAILLE_ETABLI)
            {
                position pos2{line, column + 1};
                try_pos(pos1, pos2);
                try_pos(pos2, pos1);
            }
        }
    }
    return positions;
}

void GameState::hist_add_place(position pos1, position pos2,
                               unsigned apprentice_id)
{
    assert(apprentices_.count(apprentice_id) != 0);
    action_hist a;
    a.type = ACTION_PLACER;
    a.pos1 = pos1;
    a.pos2 = pos2;
    apprentices_.at(apprentice_id).add_action(a);
}

void GameState::hist_add_transmute(position pos, unsigned apprentice_id)
{
    assert(apprentices_.count(apprentice_id) != 0);
    action_hist a;
    a.type = ACTION_TRANSMUTER;
    a.pos1 = pos;
    apprentices_.at(apprentice_id).add_action(a);
}

void GameState::hist_add_catalyze(position pos, unsigned target_apprentice_id,
                                  case_type new_type, unsigned apprentice_id)
{
    assert(apprentices_.count(apprentice_id) != 0);
    action_hist a;
    a.type = ACTION_CATALYSER;
    a.pos1 = pos;
    a.id_apprenti = target_apprentice_id;
    a.nouvelle_case = new_type;
    apprentices_.at(apprentice_id).add_action(a);
}

void GameState::reset_history(unsigned apprentice_id)
{
    assert(apprentices_.count(apprentice_id) != 0);
    apprentices_.at(apprentice_id).reset_actions();
}

const std::vector<action_hist>&
GameState::get_history(unsigned apprentice_id) const
{
    assert(apprentices_.count(apprentice_id) != 0);
    return apprentices_.at(apprentice_id).get_actions();
}

void GameState::merge_connect_components(int from, int to,
                                         unsigned internal_apprentice_id)
{
    if (from == to) // no op
        return;
    for (auto& line : connected_components_.at(internal_apprentice_id))
        for (auto& cc : line)
            if (cc == from)
                cc = to;
}

void GameState::traverse_and_update_cc(position from, position ignored,
                                       Connect_components& ccs)
{
    std::array<std::array<bool, TAILLE_ETABLI>, TAILLE_ETABLI> visited;
    visited[ignored.ligne][ignored.colonne] = true;
    int cc_from = ccs[from.ligne][from.colonne];
    int cc_to = from.ligne * TAILLE_ETABLI + from.colonne;
    std::queue<position> queue;
    queue.emplace(from);
    while (!queue.empty())
    {
        position pos = queue.front();
        queue.pop();
        if (visited[pos.ligne][pos.colonne])
            continue;
        visited[pos.ligne][pos.colonne] = true;
        if (ccs[pos.ligne][pos.colonne] != cc_from)
            continue;
        ccs[pos.ligne][pos.colonne] = cc_to;
        for (position offset : offsets)
            queue.emplace(pos + offset);
    }
}

void GameState::change_workbench_case(position pos, case_type to,
                                      unsigned internal_apprentice_id)
{
    // This function does two things. The easy part is to change the value
    // of the workbench.
    // But it also keeps up to date the connected components numbers.
    // About those numbers: an empty case always has a number equa to
    // CONNECTED_COMPONENT_OF_EMPTY, a non-empty case has the same value as all
    // the cases in its region, and is equal to the inherent value of one of
    // those cases.
    // The inherent value of a case at a position {line, column} is
    // line * TAILLE_ETABLI + column

    auto& ccs = connected_components_.at(internal_apprentice_id);
    auto& case0 = workbenches_[internal_apprentice_id][pos.ligne][pos.colonne];
    auto& cc = ccs[pos.ligne][pos.colonne];
    if (case0 != VIDE)
    {
        for (position offset : offsets)
        {
            position p = pos + offset;
            if (ccs[p.ligne][p.colonne] == cc)
                traverse_and_update_cc(p, pos, ccs);
        }
    }
    case0 = to;
    assert(to != VIDE);
    cc = pos.ligne * TAILLE_ETABLI + pos.colonne;
    for (position offset : offsets)
    {
        position p = pos + offset;
        if (workbenches_[internal_apprentice_id][p.ligne][p.colonne] == case0)
            merge_connect_components(cc, ccs[p.ligne][p.colonne],
                                     internal_apprentice_id);
    }
}
