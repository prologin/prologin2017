#ifndef TEST_HELPERS_HH
#define TEST_HELPERS_HH

#include <fstream>
#include <sstream>

#include <gtest/gtest.h>

#include "../api.hh"
#include "../constant.hh"
#include "../game_state.hh"
#include "../position.hh"
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

    virtual void TearDown()
    {
        delete gs_;
    }

    GameState* gs_;

    const int PLAYER_1 = 42;
    const int PLAYER_2 = 1337;
};

class ApiTest : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
        // Players values are not 0 and 1, because that would be too simple
        int player_id_1 = 1337;
        int player_id_2 = 42;
        utils::Logger::get().level() = utils::Logger::DEBUG_LEVEL;
        auto players_ptr = make_players(player_id_1, player_id_2);
        gs_ = new GameState(make_players(player_id_1, player_id_2));
        players[0].id = player_id_1;
        players[0].api = new Api(gs_, players_ptr->players[0]);
        players[1].id = player_id_2;
        players[1].api = new Api(gs_, players_ptr->players[1]);
    }

    virtual void TearDown()
    {
        delete players[0].api;
        delete players[1].api;
        delete gs_;
    }

    GameState* gs_;

    struct Player
    {
        int id;
        Api* api;
    };
    std::array<Player, 2> players;
};

#endif
