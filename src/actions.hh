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

#ifndef ACTIONS_HH
#define ACTIONS_HH

enum action_id
{
    ID_ACTION_PLACER_ECHANTILLON,
    ID_ACTION_TRANSMUTER,
    ID_ACTION_CATALYSER,
    ID_ACTION_DONNER_ECHANTILLON
};

#include "action_placer_echantillon.hh"
#include "action_transmuter.hh"
#include "action_catalyser.hh"
#include "action_donner_echantillon.hh"

#endif // !ACTIONS_HH
