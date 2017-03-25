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
