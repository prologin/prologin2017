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

#include <cstdlib>
#include <memory>
#include <rules/client-messenger.hh>
#include <rules/server-messenger.hh>
#include <utils/log.hh>

#include "rules.hh"

// Forward decls
namespace rules
{
struct Options;
}

static Rules* rules_;

extern "C" {

void rules_init(const rules::Options& opt)
{
    utils::Logger::get().level() = (utils::Logger::DisplayLevel)opt.verbose;
    rules_ = new Rules(opt);
}

void rules_result()
{
    delete rules_;
}

void player_loop(rules::ClientMessenger_sptr msgr)
{
    rules_->player_loop(msgr);
}

void server_loop(rules::ServerMessenger_sptr msgr)
{
    rules_->server_loop(msgr);
}

void spectator_loop(rules::ClientMessenger_sptr msgr)
{
    rules_->spectator_loop(msgr);
}

} // extern "C"
