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

int ActionTransmuter::check(const GameState* st) const
{
    if (pos_.ligne < 0 || pos_.ligne >= TAILLE_ETABLI || pos_.colonne < 0 ||
        pos_.colonne >= TAILLE_ETABLI)
        return POSITION_INVALIDE;

    if (st->get_cell_type(pos_, player_id_) == VIDE)
        return CASE_VIDE;

    return OK;
}

void ActionTransmuter::apply_on(GameState* st) const
{
    element_propriete region_prop =
        st->get_element_property(st->get_cell_type(pos_, player_id_));
    int region_size = st->remove_region(pos_, player_id_);

    if (region_prop == TRANSMUTABLE_OR)
    {
        st->increase_score(player_id_,
                           st->transmute_gold_scoreval(region_size));
    }
    else if (region_prop == TRANSMUTABLE_CATALYSEUR)
    {
        st->increase_score(player_id_,
                           st->transmute_catalyst_scoreval(region_size));
        st->increase_catalysts_number(
            st->transmute_catalyst_outcome(region_size));
    }

    st->hist_add_transmute(pos_, player_id_);
}
