#include "../actions.hh"

#include "test-helpers.hh"

// ActionDonnerEchantillon(Echantillon echantillon)

TEST_F(ActionTest, DonnerEchantillon_Invalid)
{
    ActionDonnerEchantillon act({VIDE, MERCURE}, PLAYER_1);
    EXPECT_EQ(ECHANTILLON_INCOMPLET, act.check(*gs_));

    ActionDonnerEchantillon act2({SOUFRE, VIDE}, PLAYER_2);
    EXPECT_EQ(ECHANTILLON_INCOMPLET, act2.check(*gs_));

    ActionDonnerEchantillon act3({VIDE, VIDE}, PLAYER_1);
    EXPECT_EQ(ECHANTILLON_INCOMPLET, act3.check(*gs_));
}

TEST_F(ActionTest, DonnerEchantillon_MatchingPrevious)
{
    gs_->set_next_sample({MERCURE, SOUFRE});
    gs_->reset_turn_state();

    ActionDonnerEchantillon act({MERCURE, FER}, PLAYER_2);
    EXPECT_EQ(OK, act.check(*gs_));

    ActionDonnerEchantillon act2({SOUFRE, FER}, PLAYER_2);
    EXPECT_EQ(OK, act2.check(*gs_));

    ActionDonnerEchantillon act3({CUIVRE, MERCURE}, PLAYER_2);
    EXPECT_EQ(OK, act3.check(*gs_));

    ActionDonnerEchantillon act4({CUIVRE, SOUFRE}, PLAYER_2);
    EXPECT_EQ(OK, act4.check(*gs_));

    ActionDonnerEchantillon act5({MERCURE, SOUFRE}, PLAYER_2);
    EXPECT_EQ(OK, act5.check(*gs_));

    ActionDonnerEchantillon act6({SOUFRE, MERCURE}, PLAYER_2);
    EXPECT_EQ(OK, act6.check(*gs_));

    ActionDonnerEchantillon act7({CUIVRE, FER}, PLAYER_2);
    EXPECT_EQ(ECHANTILLON_INVALIDE, act7.check(*gs_));

    ActionDonnerEchantillon act8({PLOMB, PLOMB}, PLAYER_2);
    EXPECT_EQ(ECHANTILLON_INVALIDE, act8.check(*gs_));
}

TEST_F(ActionTest, DonnerEchantillon_UpdateGamestate)
{
    gs_->set_next_sample({MERCURE, SOUFRE});
    gs_->reset_turn_state();

    ActionDonnerEchantillon act({FER, MERCURE}, PLAYER_1);
    EXPECT_EQ(OK, act.check(*gs_));
    act.apply(gs_.get());
    EXPECT_EQ(true, gs_->was_sample_given());

    ActionDonnerEchantillon act2({FER, MERCURE}, PLAYER_1);
    EXPECT_EQ(DEJA_DONNE, act2.check(*gs_));
}
