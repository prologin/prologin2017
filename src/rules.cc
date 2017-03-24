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

#include "rules.hh"
#include "actions.hh"

Rules::Rules(const rules::Options opt)
    : TurnBasedRules(opt)
    , sandbox_(opt.time)
{
    if (!opt.champion_lib.empty())
    {
        champion_dll_ = std::make_unique<utils::DLL>(opt.champion_lib);

        // FIXME: register user functions
    }

    GameState* game_state = new GameState(opt.players);
    api_ = std::make_unique<Api>(game_state, opt.player);
    register_actions();
}

void Rules::register_actions()
{
    api_->actions()->register_action(ID_ACTION_PLACER_ECHANTILLON,
                                     []() -> rules::IAction*
                                     {
                                         return new ActionPlacerEchantillon();
                                     });
    api_->actions()->register_action(ID_ACTION_TRANSMUTER,
                                     []() -> rules::IAction*
                                     {
                                         return new ActionTransmuter();
                                     });
    api_->actions()->register_action(ID_ACTION_CATALYSER,
                                     []() -> rules::IAction*
                                     {
                                         return new ActionCatalyser();
                                     });
    api_->actions()->register_action(ID_ACTION_DONNER_ECHANTILLON,
                                     []() -> rules::IAction*
                                     {
                                         return new ActionDonnerEchantillon();
                                     });
}

rules::Actions* Rules::get_actions()
{
    return api_->actions();
}

void Rules::apply_action(const rules::IAction_sptr& action)
{
    // When receiving an action, the API should have already checked that it
    // is valid. We recheck that for the current gamestate here to avoid weird
    // client/server desynchronizations and make sure the gamestate is always
    // consistent across the clients and the server.

    int err = action->check(api_->game_state());
    if (err)
        FATAL("Synchronization error: received action %d from player %d, but "
              "check() on current gamestate returned %d.",
              action->id(), action->player_id(), err);
    api_->game_state_set(action->apply(api_->game_state()));
}

bool Rules::is_finished()
{
    // FIXME
    return true;
}
