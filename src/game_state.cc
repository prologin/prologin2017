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
    // FIXME
}

rules::GameState* GameState::copy() const
{
    return new GameState(*this);
}
