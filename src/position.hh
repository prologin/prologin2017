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

#pragma once

#include "constant.hh"
#include <cstdlib>
#include <functional>

inline bool operator==(const position& a, const position& b)
{
    return a.ligne == b.ligne && a.colonne == b.colonne;
}

inline bool operator!=(const position& a, const position& b)
{
    return !(a == b);
}

inline bool operator<(const position& a, const position& b)
{
    return a.ligne < b.ligne && a.colonne < b.colonne;
}

inline position operator-(position a)
{
    return {-a.ligne, -a.colonne};
}

inline position& operator+=(position& lhs, position rhs)
{
    lhs.colonne += rhs.colonne;
    lhs.ligne += rhs.ligne;
    return lhs;
}

inline position& operator-=(position& lhs, position rhs)
{
    return lhs += -rhs;
}

inline position operator+(position lhs, position rhs)
{
    return lhs += rhs;
}

inline position operator-(position lhs, position rhs)
{
    return lhs -= rhs;
}

inline int distance(const position& a, const position& b)
{
    return std::abs(a.ligne - b.ligne) + std::abs(a.colonne - b.colonne);
}

namespace std
{
template <> struct hash<position>
{
    std::size_t operator()(const position& p) const
    {
        std::size_t s = std::hash<int>()(p.colonne) + 0x9e3779b9;
        return s ^ (std::hash<int>()(p.ligne) + (s << 6) + (s >> 2));
    }
};
}
