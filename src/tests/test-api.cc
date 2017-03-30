/*
** This file is part of Prologin2016, a rules library for stechec2.
**
** Copyright (c) 2016 Association Prologin <info@prologin.org>
**
** Prologin2016 is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Prologin2016 is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with Prologin2016.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "../api.hh"

#include "test-helpers.hh"

TEST_F(ApiTest, Api_TypeCase)
{
    // EXPECT_EQ(VIDE, players[0].api->type_case({-1, -1}, players[0].id));
    EXPECT_EQ(VIDE, players[0].api->type_case({1, 1}, players[0].id));
    gs_->set_next_sample({MERCURE, PLOMB});
    gs_->reset_turn_state();
    gs_->place_sample({1, 1}, {1, 2}, players[0].id);
    EXPECT_EQ(MERCURE, players[0].api->type_case({1, 1}, players[0].id));
    EXPECT_EQ(PLOMB, players[0].api->type_case({1, 2}, players[0].id));
    EXPECT_EQ(VIDE, players[0].api->type_case({1, 2}, players[1].id));
}

TEST_F(ApiTest, Api_EstVide)
{
    // EXPECT_TRUE(players[0].api->est_vide({-1, -1}, players[0].id));
    EXPECT_TRUE(players[0].api->est_vide({1, 1}, players[0].id));
    gs_->set_next_sample({MERCURE, PLOMB});
    gs_->reset_turn_state();
    gs_->place_sample({1, 1}, {1, 2}, players[0].id);
    EXPECT_FALSE(players[0].api->est_vide({1, 1}, players[0].id));
    EXPECT_FALSE(players[0].api->est_vide({1, 2}, players[0].id));
    EXPECT_TRUE(players[0].api->est_vide({1, 2}, players[1].id));
}

TEST_F(ApiTest, Api_ProprieteCase)
{
    // EXPECT_TRUE(players[0].api->propriete_case({-1, -1}, players[0].id));
    EXPECT_EQ(AUCUNE, players[0].api->propriete_case({1, 1}, players[0].id));
    gs_->set_next_sample({MERCURE, PLOMB});
    gs_->reset_turn_state();
    gs_->place_sample({1, 1}, {1, 2}, players[0].id);
    EXPECT_EQ(TRANSMUTABLE_CATALYSEUR,
              players[0].api->propriete_case({1, 1}, players[0].id));
    EXPECT_EQ(TRANSMUTABLE_OR,
              players[0].api->propriete_case({1, 2}, players[0].id));
    EXPECT_EQ(AUCUNE, players[0].api->propriete_case({1, 2}, players[1].id));
}

TEST_F(ApiTest, Api_ProprieteCaseType)
{
    EXPECT_EQ(AUCUNE, players[0].api->propriete_case_type(VIDE));
    EXPECT_EQ(TRANSMUTABLE_OR, players[0].api->propriete_case_type(PLOMB));
    EXPECT_EQ(TRANSMUTABLE_OR, players[0].api->propriete_case_type(FER));
    EXPECT_EQ(TRANSMUTABLE_OR, players[0].api->propriete_case_type(CUIVRE));
    EXPECT_EQ(TRANSMUTABLE_CATALYSEUR,
              players[0].api->propriete_case_type(SOUFRE));
    EXPECT_EQ(TRANSMUTABLE_CATALYSEUR,
              players[0].api->propriete_case_type(MERCURE));
}

TEST_F(ApiTest, Api_TailleRegion)
{
    EXPECT_EQ(0, players[0].api->taille_region({1, 1}, players[0].id));

    gs_->set_next_sample({MERCURE, MERCURE});
    gs_->reset_turn_state();
    gs_->place_sample({1, 1}, {1, 2}, players[0].id);
    EXPECT_EQ(2, players[0].api->taille_region({1, 1}, players[0].id));
    EXPECT_EQ(2, players[0].api->taille_region({1, 2}, players[0].id));

    gs_->set_next_sample({CUIVRE, MERCURE});
    gs_->reset_turn_state();
    gs_->place_sample({2, 1}, {2, 2}, players[0].id);
    EXPECT_EQ(1, players[0].api->taille_region({2, 1}, players[0].id));
    EXPECT_EQ(3, players[0].api->taille_region({1, 1}, players[0].id));

    gs_->set_next_sample({MERCURE, MERCURE});
    gs_->reset_turn_state();
    gs_->place_sample({3, 2}, {4, 2}, players[0].id);
    EXPECT_EQ(5, players[0].api->taille_region({4, 2}, players[0].id));
}

TEST_F(ApiTest, Api_PositionRegion)
{
    EXPECT_EQ(static_cast<unsigned int>(0),
              players[0].api->positions_region({1, 1}, players[0].id).size());

    gs_->set_next_sample({MERCURE, MERCURE});
    gs_->reset_turn_state();
    gs_->place_sample({1, 1}, {1, 2}, players[0].id);
    gs_->set_next_sample({CUIVRE, MERCURE});
    gs_->reset_turn_state();
    gs_->place_sample({2, 1}, {2, 2}, players[0].id);
    gs_->set_next_sample({MERCURE, MERCURE});
    gs_->reset_turn_state();
    gs_->place_sample({3, 2}, {4, 2}, players[0].id);

    {
        auto pos = players[0].api->positions_region({2, 1}, players[0].id);
        EXPECT_TRUE(find(pos.begin(), pos.end(), position({2, 1})) !=
                    pos.end());
        EXPECT_FALSE(find(pos.begin(), pos.end(), position({2, 2})) !=
                     pos.end());
        EXPECT_EQ(static_cast<unsigned int>(1), pos.size());
    }

    {
        auto pos = players[0].api->positions_region({1, 1}, players[0].id);
        EXPECT_FALSE(find(pos.begin(), pos.end(), position({2, 1})) !=
                     pos.end());
        EXPECT_TRUE(find(pos.begin(), pos.end(), position({2, 2})) !=
                    pos.end());
        EXPECT_TRUE(find(pos.begin(), pos.end(), position({1, 1})) !=
                    pos.end());
        EXPECT_TRUE(find(pos.begin(), pos.end(), position({1, 2})) !=
                    pos.end());
        EXPECT_TRUE(find(pos.begin(), pos.end(), position({3, 2})) !=
                    pos.end());
        EXPECT_TRUE(find(pos.begin(), pos.end(), position({4, 2})) !=
                    pos.end());
        EXPECT_EQ(static_cast<unsigned int>(5), pos.size());
    }
}

TEST_F(ApiTest, Api_PlacementsPossibleEchantillon)
{
    EXPECT_EQ(
        static_cast<unsigned int>(2 * 2 * 5 * 6),
        players[0]
            .api->placements_possible_echantillon({FER, PLOMB}, players[0].id)
            .size());

    gs_->set_next_sample({MERCURE, MERCURE});
    gs_->reset_turn_state();
    gs_->place_sample({1, 1}, {1, 2}, players[0].id);
    gs_->set_next_sample({CUIVRE, MERCURE});
    gs_->reset_turn_state();
    gs_->place_sample({2, 1}, {2, 2}, players[0].id);
    gs_->set_next_sample({MERCURE, MERCURE});
    gs_->reset_turn_state();
    gs_->place_sample({3, 2}, {4, 2}, players[0].id);

    EXPECT_EQ(static_cast<unsigned int>(40),
              players[0]
                  .api->placements_possible_echantillon({MERCURE, MERCURE},
                                                        players[0].id)
                  .size());
    EXPECT_EQ(static_cast<unsigned int>(23),
              players[0]
                  .api->placements_possible_echantillon({MERCURE, CUIVRE},
                                                        players[0].id)
                  .size());
    EXPECT_EQ(static_cast<unsigned int>(23),
              players[0]
                  .api->placements_possible_echantillon({CUIVRE, MERCURE},
                                                        players[0].id)
                  .size());
    EXPECT_EQ(static_cast<unsigned int>(4),
              players[0]
                  .api->placements_possible_echantillon({CUIVRE, CUIVRE},
                                                        players[0].id)
                  .size());
    EXPECT_EQ(
        static_cast<unsigned int>(0),
        players[0]
            .api->placements_possible_echantillon({FER, PLOMB}, players[0].id)
            .size());
    EXPECT_EQ(
        static_cast<unsigned int>(2),
        players[0]
            .api->placements_possible_echantillon({FER, CUIVRE}, players[0].id)
            .size());
}
