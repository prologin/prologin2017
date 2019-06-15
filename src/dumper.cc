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

#include <iomanip>
#include <iostream>
#include <string>

#include "api.hh"
#include "constant.hh"
#include "game_state.hh"
#include "rules.hh"

constexpr auto COMMA = ", ";

/// Decodes a UTF-8 string to a list of 32 bit unicode codepoints. Ignores
/// erroneous characters.
/// Imported from prologin2016
static std::u32string utf8_decode(const std::string& s)
{
    std::u32string ret;
    size_t i = 0;
    size_t size = s.size();

    while (i < size)
    {
        if ((s[i] & 0x80) == 0)
        {
            ret.push_back(s[i++]);
        }
        else if ((s[i] & 0xe0) == 0xc0 && (i + 1) < size &&
                 (s[i + 1] & 0xc0) == 0x80)
        {
            ret.push_back(((s[i] & 0x1f) << 6) | (s[i + 1] & 0x3f));
            i += 2;
        }
        else if ((s[i] & 0xf0) == 0xe0 && (i + 2) < size &&
                 (s[i + 1] & 0xc0) == 0x80 && (s[i + 2] & 0xc0) == 0x80)
        {
            ret.push_back(((s[i] & 0x0f) << 12) | ((s[i + 1] & 0x3f) << 6) |
                          (s[i + 2] & 0x3f));
            i += 3;
        }
        else if ((s[i] & 0xf8) == 0xf0 && (i + 3) < size &&
                 (s[i + 1] & 0xc0) == 0x80 && (s[i + 2] & 0xc0) == 0x80 &&
                 (s[i + 1] & 0xc0) == 0x80)
        {
            ret.push_back(((s[i] & 0x07) << 18) | ((s[i + 1] & 0x3f) << 12) |
                          ((s[i + 2] & 0x3f) << 6) | (s[i + 3] & 0x3f));
            i += 4;
        }
        else
        {
            i++;
        }
    }

    return ret;
}

/// Dump a JSON-escaped UTF-8 string
/// Imported from prologin2016
static void dump_string(std::ostream& ss, const std::string& s)
{
    /*
     * RFC4627, 2.5:
     * All Unicode characters may be placed within the quotation marks except
     * for the characters that must be escaped: quotation mark, reverse solidus,
     * and the control characters (U+0000 through U+001F).
     */
    std::ios state(nullptr);
    state.copyfmt(ss);
    std::u32string utf32 = utf8_decode(s);
    ss << "\"";
    for (char32_t& c : utf32)
    {
        if (c == u'"')
        {
            ss << "\\\"";
        }
        else if (c == u'\\')
        {
            ss << "\\\\";
        }
        else if (u'\u0020' <= c && c <= u'\u007E')
        {
            // printable ASCII
            ss << static_cast<char>(c);
        }
        else if (c > u'\uFFFF')
        {
            // surrogate pair
            // http://unicode.org/faq/utf_bom.html#utf16-2
            const unsigned s = c - 0x010000;
            const unsigned lead = (s >> 10) + 0xD800;
            const unsigned trail = (s & 0x3FF) + 0xDC00;
            ss << "\\u" << std::hex << std::setfill('0') << std::setw(4)
               << lead;
            ss.copyfmt(state);
            ss << "\\u" << std::hex << std::setfill('0') << std::setw(4)
               << trail;
            ss.copyfmt(state);
        }
        else
        {
            ss << "\\u" << std::hex << std::setfill('0') << std::setw(4) << c;
            ss.copyfmt(state);
        }
    }
    ss << "\"";
}

static std::ostream& operator<<(std::ostream& ss, const position& pos)
{
    ss << "{\"r\": " << pos.ligne << ", \"c\":" << pos.colonne << "}";
    return ss;
}

static std::ostream& operator<<(std::ostream& ss, action_type act)
{
    ss << "\"";
    switch (act)
    {
    case ACTION_PLACER:
        ss << "ACTION_PLACER";
        break;
    case ACTION_TRANSMUTER:
        ss << "ACTION_TRANSMUTER";
        break;
    case ACTION_CATALYSER:
        ss << "ACTION_CATALYSER";
        break;
    case ACTION_DONNER_ECHANTILLON:
        ss << "ACTION_DONNER_ECHANTILLON";
        break;
    }
    ss << "\"";
    return ss;
}

static std::ostream& operator<<(std::ostream& ss, const case_type& ctype)
{
    ss << "\"";
    switch (ctype)
    {
    case VIDE:
        ss << "VIDE";
        break;
    case PLOMB:
        ss << "PLOMB";
        break;
    case FER:
        ss << "FER";
        break;
    case CUIVRE:
        ss << "CUIVRE";
        break;
    case SOUFRE:
        ss << "SOUFRE";
        break;
    case MERCURE:
        ss << "MERCURE";
        break;
    }
    ss << "\"";
    return ss;
}

/// Dump one of the benches
static void dump_bench(std::ostream& ss, const GameState& st, unsigned id)
{
    ss << "[";
    for (int row = 0; row < TAILLE_ETABLI; row++)
    {
        for (int col = 0; col < TAILLE_ETABLI; col++)
        {
            position pos{row, col};
            ss << st.get_cell_type(pos, id);
            if (!(row == TAILLE_ETABLI - 1 && col == TAILLE_ETABLI - 1))
                ss << ", ";
        }
    }
    ss << "]";
}

static void dump_history(std::ostream& ss, const GameState& st, unsigned id)
{
    const std::vector<action_hist>& history = st.get_history(id);

    auto sep = "";
    ss << "[";
    for (const auto& action : history)
    {
        ss << sep;
        sep = COMMA;
        ss << "{\"type\": " << action.atype << ", ";

        switch (action.atype)
        {
        case ACTION_PLACER:
            ss << "\"pos1\": " << action.pos1 << ", "
               << "\"pos2\": " << action.pos2;
            break;
        case ACTION_TRANSMUTER:
            ss << "\"pos\": " << action.pos1;
            break;
        case ACTION_CATALYSER:
            ss << "\"pos\": " << action.pos1 << ", "
               << "\"apprentice_id\": " << action.id_apprenti << ", "
               << "\"new_type\": " << action.nouvelle_case;
            break;
        case ACTION_DONNER_ECHANTILLON:
            // This action is not represented in the history
            break;
        }
        ss << "}";
    }
    ss << "]";
}

/// Dumps the players' data
static void dump_players(std::ostream& ss, const GameState& st)
{
    const auto& players = st.get_apprentices();

    auto sep = "";
    ss << "{";
    for (const auto& player_entry : players)
    {
        ss << sep;
        sep = COMMA;

        int id = player_entry.first;
        const auto& player = player_entry.second;
        ss << "\"" << id << "\": {"
           << "\"name\": ";
        dump_string(ss, player.get_name());
        ss << ", \"score\": " << player.get_score() << ", ";

        ss << "\"bench\": ";
        dump_bench(ss, st, id);

        ss << ", \"just_played\": ";
        // This relies on `set_current_player` being called at the
        // *BEGINNING* of a player's turn
        if (st.current_player() == id)
        {
            ss << "true";

            const echantillon& sample = st.current_sample();
            ss << ", \"sample\": [" << sample.element1 << ", "
               << sample.element2 << "]";
        }
        else
            ss << "false";

        ss << ", \"history\": ";
        dump_history(ss, st, id);

        ss << "}";
    }
    ss << "}";
}

/// Dump the whole gamestate
static void dump_stream(std::ostream& ss, const GameState& st)
{
    ss << "{";

    ss << "\"turn\": [" << st.get_turn() << ", " << NB_TOURS << "], ";

    ss << "\"players\": ";
    dump_players(ss, st);

    ss << "}\n";
}

void Rules::dump_state(std::ostream& ss)
{
    dump_stream(ss, *api_->game_state());
}

// from api.cc
extern Api* api;

extern "C" const char* dump_state_json()
{
    // Warning: everytime this function is called, it invalidates the previous
    // return values by free-ing them.
    static std::string s;
    std::ostringstream ss;
    dump_stream(ss, *api->game_state());
    s = ss.str();
    return s.c_str();
}
