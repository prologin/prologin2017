#include "../actions.hh"

#include "test-helpers.hh"

// ActionDonnerEchantillon(Echantillon echantillon)

TEST_F(ActionTest, DonnerEchantillon_Invalid)
{
    ActionDonnerEchantillon* act;

    act = new ActionDonnerEchantillon({VIDE, MERCURE}, PLAYER_1);
    EXPECT_EQ(ECHANTILLON_INCOMPLET, act->check(gs_));
    delete act;

    act = new ActionDonnerEchantillon({SOUFRE, VIDE}, PLAYER_2);
    EXPECT_EQ(ECHANTILLON_INCOMPLET, act->check(gs_));
    delete act;

    act = new ActionDonnerEchantillon({VIDE, VIDE}, PLAYER_1);
    EXPECT_EQ(ECHANTILLON_INCOMPLET, act->check(gs_));
    delete act;
}
