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

#include "actions.hh"
#include "position.hh"

int ActionPlacerEchantillon::check(const GameState* st) const
{
    if(st->is_sample_placed())
        return DEJA_POSE;

    if(!in_board(pos1_) || !in_board(pos2_))
        return POSITION_INVALIDE;

    if(distance(pos1_, pos2_) != 1)
        return PLACEMENT_INVALIDE;

    if (st->get_cell_type(pos1_, player_id_) != VIDE ||
        st->get_cell_type(pos2_, player_id_) != VIDE)
        return PLACEMENT_IMPOSSIBLE;

    const echantillon& sample = st->current_sample();

    if(!st->is_valid_sample_position(sample, pos1_, pos2_, player_id_))
        return PLACEMENT_INCORRECT;

    return OK;
}

void ActionPlacerEchantillon::apply_on(GameState* st) const
{
    st->place_sample(pos1_, pos2_, player_id_);
    st->hist_add_place(pos1_, pos2_, player_id_);
}
