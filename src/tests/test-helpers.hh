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

static rules::Players make_players(int id1, int id2)
{
    /* Create two players (no spectator).  */
    rules::Players players;
    players.add(std::make_shared<rules::Player>(id1, rules::PLAYER));
    players.add(std::make_shared<rules::Player>(id2, rules::PLAYER));
    return players;
}

class ActionTest : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
        utils::Logger::get().level() = utils::Logger::DEBUG_LEVEL;
        gs_.reset(new GameState(make_players(PLAYER_1, PLAYER_2)));
    }

    std::unique_ptr<GameState> gs_;

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
        auto gs_players = make_players(player_id_1, player_id_2);
        auto gs =
            std::make_unique<GameState>(make_players(player_id_1, player_id_2));
        players[0].id = player_id_1;
        players[0].api.reset(new Api(std::unique_ptr<GameState>(gs->copy()),
                                     gs_players[0]));
        players[1].id = player_id_2;
        players[1].api.reset(new Api(std::unique_ptr<GameState>(gs->copy()),
                                     gs_players[1]));
    }

    GameState* gs() { return &players[0].api->game_state(); }

    struct Player
    {
        int id;
        std::unique_ptr<Api> api;
    };
    std::array<Player, 2> players;
};

#endif
