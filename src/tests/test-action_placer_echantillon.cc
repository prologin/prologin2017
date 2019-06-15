#include "../actions.hh"

#include "test-helpers.hh"

// ActionPlacerEchantillon(position pos1, position pos2)

TEST_F(ActionTest, PlacerEchantillon_InvalidCell)
{
    ActionPlacerEchantillon act({-1, -1}, {0, -1}, PLAYER_1);
    EXPECT_EQ(POSITION_INVALIDE, act.check(*gs_));
}

TEST_F(ActionTest, PlacerEchantillon_NotAdjacent)
{
    ActionPlacerEchantillon act({1, 1}, {2, 2}, PLAYER_1);
    EXPECT_EQ(PLACEMENT_INVALIDE, act.check(*gs_));

    ActionPlacerEchantillon act2({1, 1}, {1, 1}, PLAYER_1);
    EXPECT_EQ(PLACEMENT_INVALIDE, act2.check(*gs_));
}

TEST_F(ActionTest, PlacerEchantillon_Impossible)
{
    gs_->set_next_sample({MERCURE, FER});
    gs_->reset_turn_state();

    ActionPlacerEchantillon act({1, 1}, {2, 1}, PLAYER_1);
    EXPECT_EQ(OK, act.check(*gs_));
    act.apply(gs_.get());

    gs_->set_next_sample({CUIVRE, PLOMB});
    gs_->reset_turn_state();

    ActionPlacerEchantillon act2({2, 2}, {3, 2}, PLAYER_1);
    EXPECT_EQ(OK, act2.check(*gs_));

    ActionPlacerEchantillon act3({1, 1}, {2, 1}, PLAYER_1);
    EXPECT_EQ(PLACEMENT_IMPOSSIBLE, act3.check(*gs_));

    ActionPlacerEchantillon act4({1, 1}, {1, 2}, PLAYER_1);
    EXPECT_EQ(PLACEMENT_IMPOSSIBLE, act4.check(*gs_));

    ActionPlacerEchantillon act5({2, 2}, {2, 1}, PLAYER_1);
    EXPECT_EQ(PLACEMENT_IMPOSSIBLE, act5.check(*gs_));

    ActionPlacerEchantillon act6({2, 1}, {1, 1}, PLAYER_1);
    EXPECT_EQ(PLACEMENT_IMPOSSIBLE, act6.check(*gs_));
}

TEST_F(ActionTest, PlacerEchantillon_Connexity)
{
    gs_->set_next_sample({MERCURE, FER});
    gs_->reset_turn_state();

    ActionPlacerEchantillon act({1, 1}, {2, 1}, PLAYER_1);
    EXPECT_EQ(OK, act.check(*gs_));
    act.apply(gs_.get());

    gs_->set_next_sample({MERCURE, PLOMB});
    gs_->reset_turn_state();

    ActionPlacerEchantillon act2({2, 2}, {3, 2}, PLAYER_1);
    EXPECT_EQ(PLACEMENT_INCORRECT, act2.check(*gs_));

    ActionPlacerEchantillon act3({1, 2}, {2, 2}, PLAYER_1);
    EXPECT_EQ(OK, act3.check(*gs_));

    ActionPlacerEchantillon act4({0, 1}, {0, 0}, PLAYER_1);
    EXPECT_EQ(OK, act4.check(*gs_));

    ActionPlacerEchantillon act5({0, 0}, {0, 1}, PLAYER_1);
    EXPECT_EQ(PLACEMENT_INCORRECT, act5.check(*gs_));

    ActionPlacerEchantillon act6({2, 2}, {1, 2}, PLAYER_1);
    EXPECT_EQ(PLACEMENT_INCORRECT, act6.check(*gs_));
}

TEST_F(ActionTest, PlacerEchantillon_AlreadyGiven)
{
    gs_->set_next_sample({MERCURE, FER});
    gs_->reset_turn_state();

    {
        ActionPlacerEchantillon act({1, 1}, {2, 1}, PLAYER_1);
        EXPECT_EQ(OK, act.check(*gs_));
        act.apply(gs_.get());
        EXPECT_EQ(true, gs_->is_sample_placed());
    }

    {
        ActionPlacerEchantillon act({1, 2}, {2, 2}, PLAYER_1);
        EXPECT_EQ(DEJA_POSE, act.check(*gs_));
    }
}

TEST_F(ActionTest, PlacerEchantillon_Forgot)
{
    gs_->set_next_sample({MERCURE, MERCURE});
    gs_->reset_turn_state();

    {
        ActionPlacerEchantillon act({1, 1}, {2, 1}, PLAYER_1);
        EXPECT_EQ(OK, act.check(*gs_));
        act.apply(gs_.get());
        EXPECT_EQ(true, gs_->is_sample_placed());
    }

    EXPECT_EQ(MERCURE, gs_->get_cell_type({1, 1}, PLAYER_1));
    EXPECT_EQ(MERCURE, gs_->get_cell_type({2, 1}, PLAYER_1));
    EXPECT_EQ(2, gs_->get_region_size({1, 1}, PLAYER_1));
    EXPECT_EQ(2, gs_->get_region_size({2, 1}, PLAYER_1));

    gs_->reset_turn_state();

    gs_->end_turn(PLAYER_1); // Should wipe `PLAYER_1`'s bench
    gs_->reset_turn_state();

    EXPECT_EQ(VIDE, gs_->get_cell_type({1, 1}, PLAYER_1));
    EXPECT_EQ(VIDE, gs_->get_cell_type({2, 1}, PLAYER_1));
    EXPECT_EQ(0, gs_->get_region_size({1, 1}, PLAYER_1));
    EXPECT_EQ(0, gs_->get_region_size({2, 1}, PLAYER_1));
}
