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

int ActionCatalyser::check(const GameState& st) const
{
    if (st.get_catalysts_number() <= 0)
        return AUCUN_CATALYSEUR;

    if (!in_board(pos_))
        return POSITION_INVALIDE;

    if (terrain_ == VIDE || terrain_ > MERCURE)
        return CATALYSE_INVALIDE;

    case_type old_cell = st.get_cell_type(pos_, id_apprenti_);
    if (old_cell == VIDE)
        return CASE_VIDE;

    return OK;
}

void ActionCatalyser::apply_on(GameState* st) const
{
    st->consume_catalyst();
    st->change_case(pos_, id_apprenti_, terrain_);
    st->hist_add_catalyze(pos_, id_apprenti_, terrain_, player_id_);
}
