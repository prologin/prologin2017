#include "../actions.hh"

#include "test-helpers.hh"

// ActionPlacerEchantillon(position pos1, position pos2)

TEST_F(ActionTest, PlacerEchantillon_InvalidCell)
{
    ActionPlacerEchantillon act({-1, -1}, {0, -1}, PLAYER_1);
    EXPECT_EQ(POSITION_INVALIDE, act.check(gs_));
}

TEST_F(ActionTest, PlacerEchantillon_NotAdjacent)
{
    ActionPlacerEchantillon act({1, 1}, {2, 2}, PLAYER_1);
    EXPECT_EQ(PLACEMENT_INVALIDE, act.check(gs_));

    ActionPlacerEchantillon act2({1, 1}, {1, 1}, PLAYER_1);
    EXPECT_EQ(PLACEMENT_INVALIDE, act2.check(gs_));
}

TEST_F(ActionTest, PlacerEchantillon_Impossible)
{
    ActionPlacerEchantillon act({1, 1}, {2, 1}, PLAYER_1);
    EXPECT_EQ(OK, act.check(gs_));
    act.apply_on(gs_);

    gs_->reset_turn_state();

    ActionPlacerEchantillon act2({2, 2}, {3, 2}, PLAYER_1);
    EXPECT_EQ(OK, act2.check(gs_));

    ActionPlacerEchantillon act3({1, 1}, {2, 1}, PLAYER_1);
    EXPECT_EQ(PLACEMENT_IMPOSSIBLE, act3.check(gs_));

    ActionPlacerEchantillon act4({1, 1}, {1, 2}, PLAYER_1);
    EXPECT_EQ(PLACEMENT_IMPOSSIBLE, act4.check(gs_));

    ActionPlacerEchantillon act5({2, 2}, {2, 1}, PLAYER_1);
    EXPECT_EQ(PLACEMENT_IMPOSSIBLE, act5.check(gs_));

    ActionPlacerEchantillon act6({2, 1}, {1, 1}, PLAYER_1);
    EXPECT_EQ(PLACEMENT_IMPOSSIBLE, act6.check(gs_));
}
