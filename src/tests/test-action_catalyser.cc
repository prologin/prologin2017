#include "../actions.hh"

#include "test-helpers.hh"

// ActionCatalyser(position pos, int id, case_type element)

TEST_F(ActionTest, Catalyser_NoCatalyst)
{
    gs_->reset_turn_state();
    gs_->place_sample({1, 1}, {1, 2}, PLAYER_2);

    {
        ActionCatalyser act({1, 1}, PLAYER_2, MERCURE, PLAYER_1);
        EXPECT_EQ(AUCUN_CATALYSEUR, act.check(gs_));
    }
}

TEST_F(ActionTest, Catalyser_InvalidPosition)
{
    gs_->increase_catalysts_number(1);

    {
        ActionCatalyser act({-1, -1}, PLAYER_2, MERCURE, PLAYER_1);
        EXPECT_EQ(POSITION_INVALIDE, act.check(gs_));
    }
}

TEST_F(ActionTest, Catalyser_Ok)
{
    gs_->reset_turn_state();
    gs_->place_sample({1, 1}, {1, 2}, PLAYER_2);
    gs_->increase_catalysts_number(1);

    {
        ActionCatalyser act({1, 1}, PLAYER_2, MERCURE, PLAYER_1);
        EXPECT_EQ(OK, act.check(gs_));
    }
}

TEST_F(ActionTest, Catalyser_EmptyCell)
{
    gs_->reset_turn_state();
    gs_->place_sample({1, 1}, {1, 2}, PLAYER_2);
    gs_->increase_catalysts_number(1);

    {
        ActionCatalyser act({2, 2}, PLAYER_2, MERCURE, PLAYER_1);
        EXPECT_EQ(CASE_VIDE, act.check(gs_));
    }

    {
        ActionCatalyser act({1, 1}, PLAYER_1, MERCURE, PLAYER_1);
        EXPECT_EQ(CASE_VIDE, act.check(gs_));
    }
}
