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

#ifndef ACTION_PLACER_ECHANTILLON_HH
#define ACTION_PLACER_ECHANTILLON_HH

#include <rules/action.hh>

#include "actions.hh"
#include "constant.hh"
#include "game_state.hh"

class ActionPlacerEchantillon : public rules::Action<GameState>
{
public:
    ActionPlacerEchantillon(position pos1, position pos2, int player_id)
        : pos1_(pos1)
        , pos2_(pos2)
        , player_id_(player_id)
    {
    }
    ActionPlacerEchantillon() {} // for register_action()

    int check(const GameState& st) const override;
    void apply_on(GameState* st) const override;

    void handle_buffer(utils::Buffer& buf) override
    {
        buf.handle(pos1_);
        buf.handle(pos2_);
        buf.handle(player_id_);
    }

    uint32_t player_id() const override { return player_id_; };
    uint32_t id() const override { return ID_ACTION_PLACER_ECHANTILLON; }

private:
    position pos1_;
    position pos2_;
    int player_id_;
};

#endif // !ACTION_PLACER_ECHANTILLON_HH
