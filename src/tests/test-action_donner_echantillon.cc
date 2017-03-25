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

TEST_F(ActionTest, DonnerEchantillon_MatchingPrevious)
{
    ActionDonnerEchantillon* act;

    gs_->set_next_sample({MERCURE, SOUFRE});
    gs_->reset_turn_state();

    act = new ActionDonnerEchantillon({MERCURE, FER}, PLAYER_2);
    EXPECT_EQ(OK, act->check(gs_));
    delete act;

    act = new ActionDonnerEchantillon({SOUFRE, FER}, PLAYER_2);
    EXPECT_EQ(OK, act->check(gs_));
    delete act;

    act = new ActionDonnerEchantillon({CUIVRE, MERCURE}, PLAYER_2);
    EXPECT_EQ(OK, act->check(gs_));
    delete act;

    act = new ActionDonnerEchantillon({CUIVRE, SOUFRE}, PLAYER_2);
    EXPECT_EQ(OK, act->check(gs_));
    delete act;

    act = new ActionDonnerEchantillon({MERCURE, SOUFRE}, PLAYER_2);
    EXPECT_EQ(OK, act->check(gs_));
    delete act;

    act = new ActionDonnerEchantillon({SOUFRE, MERCURE}, PLAYER_2);
    EXPECT_EQ(OK, act->check(gs_));
    delete act;

    act = new ActionDonnerEchantillon({CUIVRE, FER}, PLAYER_2);
    EXPECT_EQ(ECHANTILLON_INVALIDE, act->check(gs_));
    delete act;

    act = new ActionDonnerEchantillon({PLOMB, PLOMB}, PLAYER_2);
    EXPECT_EQ(ECHANTILLON_INVALIDE, act->check(gs_));
    delete act;
}

TEST_F(ActionTest, DonnerEchantillon_UpdateGamestate)
{
    ActionDonnerEchantillon* act;

    gs_->set_next_sample({MERCURE, SOUFRE});
    gs_->reset_turn_state();

    act = new ActionDonnerEchantillon({FER, MERCURE}, PLAYER_1);
    EXPECT_EQ(OK, act->check(gs_));
    act->apply_on(gs_);
    EXPECT_EQ(true, gs_->was_sample_given());
    delete act;

    act = new ActionDonnerEchantillon({FER, MERCURE}, PLAYER_1);
    EXPECT_EQ(DEJA_DONNE, act->check(gs_));
    delete act;
}
