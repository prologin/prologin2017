#ifndef TEST_HELPERS_HH
#define TEST_HELPERS_HH

#include <sstream>
#include <fstream>

#include <gtest/gtest.h>

#include "../api.hh"
#include "../constant.hh"
#include "../game_state.hh"
#include "../rules.hh"

static rules::Players_sptr make_players(int id1, int id2)
{
    /* Create two players (no spectator).  */
    return rules::Players_sptr(
        new rules::Players{std::vector<rules::Player_sptr>{
            rules::Player_sptr(new rules::Player(id1, rules::PLAYER)),
            rules::Player_sptr(new rules::Player(id2, rules::PLAYER)),
        }});
}

class ActionTest : public ::testing::Test
{
    protected:
        virtual void SetUp()
        {
            utils::Logger::get().level() = utils::Logger::DEBUG_LEVEL;
            gs_ = new GameState(make_players(PLAYER_1, PLAYER_2));
        }

        GameState* gs_;

        const int PLAYER_1 = 0;
        const int PLAYER_2 = 1;
};

#endif
