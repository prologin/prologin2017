#include "../actions.hh"

#include "test-helpers.hh"

// ActionTransmuter(position pos, int player)

TEST_F(ActionTest, Transmuter_InvalidPosition)
{
    {
    ActionTransmuter act({-1, -1}, PLAYER_1);
    EXPECT_EQ(POSITION_INVALIDE, act.check(gs_));
    }
}

TEST_F(ActionTest, Transmuter_NoElement)
{
    {
    ActionTransmuter act({2, 2}, PLAYER_1);
    EXPECT_EQ(CASE_VIDE, act.check(gs_));
    }
}

TEST_F(ActionTest, Transmuter_Ok)
{
    gs_->place_sample({1, 1}, {1, 2}, PLAYER_1);

    {
    ActionTransmuter act({1, 1}, PLAYER_1);
    EXPECT_EQ(OK, act.check(gs_));
    }
}
