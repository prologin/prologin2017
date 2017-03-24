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

int ActionDonnerEchantillon::check(const GameState* st) const
{
    if(echantillon_donne_.element1 == VIDE
            || echantillon_donne_.element2 == VIDE)
        return ECHANTILLON_INCOMPLET;

    const echantillon& received = st->current_sample();
    if(received.element1 != echantillon_donne_.element1
            && received.element1 != echantillon_donne_.element2
            && received.element2 != echantillon_donne_.element1
            && received.element2 != echantillon_donne_.element2)
        return ECHANTILLON_INVALIDE;

    return OK;
}

void ActionDonnerEchantillon::apply_on(GameState* st) const
{
    st->set_next_sample(echantillon_donne_);
}
