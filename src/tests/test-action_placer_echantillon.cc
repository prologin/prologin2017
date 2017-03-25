#include "../actions.hh"

#include "test-helpers.hh"

// ActionPlacerEchantillon(position pos1, position pos2)

TEST_F(ActionTest, PlacerEchantillon_InvalidCell)
{
    ActionPlacerEchantillon* act;

    act = new ActionPlacerEchantillon({-1, -1}, {0, -1}, PLAYER_1);
    EXPECT_EQ(POSITION_INVALIDE, act->check(gs_));
    delete act;
}

TEST_F(ActionTest, PlacerEchantillon_NotAdjacent)
{
    ActionPlacerEchantillon* act;

    act = new ActionPlacerEchantillon({1, 1}, {2, 2}, PLAYER_1);
    EXPECT_EQ(PLACEMENT_INVALIDE, act->check(gs_));
    delete act;

    act = new ActionPlacerEchantillon({1, 1}, {1, 1}, PLAYER_1);
    EXPECT_EQ(PLACEMENT_INVALIDE, act->check(gs_));
    delete act;
}

TEST_F(ActionTest, PlacerEchantillon_Impossible)
{
    ActionPlacerEchantillon* act;

    act = new ActionPlacerEchantillon({1, 1}, {2, 1}, PLAYER_1);
    EXPECT_EQ(OK, act->check(gs_));
    act->apply_on(gs_);
    delete act;

    gs_->reset_turn_state();

    act = new ActionPlacerEchantillon({2, 2}, {3, 2}, PLAYER_1);
    EXPECT_EQ(OK, act->check(gs_));
    delete act;

    act = new ActionPlacerEchantillon({1, 1}, {2, 1}, PLAYER_1);
    EXPECT_EQ(PLACEMENT_IMPOSSIBLE, act->check(gs_));
    delete act;

    act = new ActionPlacerEchantillon({1, 1}, {1, 2}, PLAYER_1);
    EXPECT_EQ(PLACEMENT_IMPOSSIBLE, act->check(gs_));
    delete act;

    act = new ActionPlacerEchantillon({2, 2}, {2, 1}, PLAYER_1);
    EXPECT_EQ(PLACEMENT_IMPOSSIBLE, act->check(gs_));
    delete act;

    act = new ActionPlacerEchantillon({2, 1}, {1, 1}, PLAYER_1);
    EXPECT_EQ(PLACEMENT_IMPOSSIBLE, act->check(gs_));
    delete act;

}
