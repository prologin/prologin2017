/*
** This file is part of Prologin2017, a rules library for stechec2.
**
** Copyright (c) 2017 Association Prologin <info@prologin.org>
**
** Prologin2017 is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Prologin2017 is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with Prologin2017.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>

#include "actions.hh"
#include "api.hh"
#include "position.hh"
#include "utils.hh"

// global used in interface.cc
Api* api;

Api::Api(std::unique_ptr<GameState> game_state,
         std::shared_ptr<rules::Player> player)
    : rules::Api<GameState, erreur>(std::move(game_state), player)
{
    api = this;
}

/// Renvoie le type d’une case donnée, ou 0 si la case est invaide.
case_type Api::type_case(position pos, int id_apprenti)
{
    if (!in_board(pos) || !game_state_->valid_player(id_apprenti))
        return VIDE;

    return game_state_->get_cell_type(pos, id_apprenti);
}

/// Indique si une case donnée est vide ou contient un élément. Renvoie faux en
/// cas d'erreur.
bool Api::est_vide(position pos, int id_apprenti)
{
    if (!in_board(pos) || !game_state_->valid_player(id_apprenti))
        return false;

    return type_case(pos, id_apprenti) == VIDE;
}

/// Renvoie la propriété de l’élément sur une case donnée. Un élément invalide
/// n'a pas de propriété.
element_propriete Api::propriete_case(position pos, int id_apprenti)
{
    if (!in_board(pos) || !game_state_->valid_player(id_apprenti))
        return AUCUNE;

    return propriete_case_type(type_case(pos, id_apprenti));
}

/// Renvoie la propriété d’un type de case donné.
element_propriete Api::propriete_case_type(case_type ctype)
{
    return game_state_->get_element_property(ctype);
}

/// Renvoie la taille de la région à laquelle appartient un élément. Renvoie -1
/// si la position est invalide.
int Api::taille_region(position pos, int id_apprenti)
{
    if (!in_board(pos) || !game_state_->valid_player(id_apprenti))
        return -1;

    return game_state_->get_region_size(pos, id_apprenti);
}

/// Renvoie la liste des positions des cases composant la région à laquelle
/// appartient un élément donné. Renvoie une liste vide en cas d'erreur.
std::vector<position> Api::positions_region(position pos, int id_apprenti)
{
    if (!in_board(pos) || !game_state_->valid_player(id_apprenti))
        return std::vector<position>();

    return game_state_->get_region(pos, id_apprenti);
}

/// Détermine si le placement d’un échantillon est valide.
bool Api::placement_possible_echantillon(echantillon echantillon_a_placer,
                                         position pos1, position pos2,
                                         int id_apprenti)
{
    if (echantillon_a_placer.element1 == VIDE ||
        echantillon_a_placer.element2 == VIDE || !in_board(pos1) ||
        !in_board(pos2) || !game_state_->valid_player(id_apprenti))
    {
        return false;
    }
    return game_state_->is_valid_sample_position(echantillon_a_placer, pos1,
                                                 pos2, id_apprenti);
}

/// Renvoie la liste des placements possibles pour un échantillon donné sur
/// l’établi d’un apprenti donné. Renvoie une liste vide en cas d'erreur.
std::vector<position_echantillon>
Api::placements_possible_echantillon(echantillon echantillon_a_placer,
                                     int id_apprenti)
{
    if (echantillon_a_placer.element1 == VIDE ||
        echantillon_a_placer.element2 == VIDE ||
        !game_state_->valid_player(id_apprenti))
    {
        return std::vector<position_echantillon>();
    }

    return game_state_->possible_sample_positions(echantillon_a_placer,
                                                  id_apprenti);
}

/// Renvoie la liste des actions jouées par l’adversaire pendant son tour, dans
/// l’ordre chronologique.
std::vector<action_hist> Api::historique()
{
    return game_state_->get_history(adversaire());
}

/// Renvoie votre numéro d’apprenti.
int Api::moi()
{
    return player_->id;
}

/// Renvoie le numéro d’apprenti de votre adversaire.
int Api::adversaire()
{
    return game_state_->opponent(moi());
}

/// Renvoie la quantité d’or amassée par l’apprenti désigné par le numéro
/// ``id_apprenti``. Renvoie 0 si ``id_apprenti`` est invalide (attention, le
/// score d’un apprenti valide peut aussi être 0).
int Api::score(int id_apprenti)
{
    if (!game_state_->valid_player(id_apprenti))
        return 0;

    return game_state_->get_score(id_apprenti);
}

/// Renvoie le numéro du tour actuel.
int Api::tour_actuel()
{
    return game_state_->get_turn();
}

/// Annule la dernière action. Renvoie ``false`` quand il n’y a pas d’action à
/// annuler ce tour-ci.
bool Api::annuler()
{
    if (!game_state_.can_cancel())
        return false;
    actions_.cancel();
    game_state_.cancel();
    return true;
}

/// Indique le nombre de catalyseurs en votre possession.
int Api::nombre_catalyseurs()
{
    return game_state_->get_catalysts_number();
}

/// Indique l’échantillon reçu pour ce tour.
echantillon Api::echantillon_tour()
{
    return game_state_->current_sample();
}

/// Indique si l’échantillon reçu pour ce tour a déjà été posé.
bool Api::a_pose_echantillon()
{
    return game_state_->is_sample_placed();
}

/// Indique si un échantillon a déjà été donné ce tour.
bool Api::a_donne_echantillon()
{
    return game_state_->was_sample_given();
}

/// Renvoie la quantité d’or (et donc le score) obtenue par la transmutation de
/// ``taille_region`` éléments transmutables en or.
int Api::quantite_transmutation_or(int taille_region)
{
    // Strict check, in case the values are defined explicitly
    if (taille_region <= 0 || taille_region > TAILLE_ETABLI * TAILLE_ETABLI)
        return 0;

    return game_state_->transmute_gold_scoreval(taille_region);
}

/// Renvoie la quantité de catalyseurs obtenue par la transmutation de
/// ``taille_region`` éléments transmutables en catalyseur.
int Api::quantite_transmutation_catalyseur(int taille_region)
{
    // Strict check, in case the values are defined explicitly
    if (taille_region <= 0 || taille_region > TAILLE_ETABLI * TAILLE_ETABLI)
        return 0;

    return game_state_->transmute_catalyst_outcome(taille_region);
}

/// Renvoie la quantité d’or obtenue par la transmutation de ``taille_region``
/// éléments transmutables en catalyseur.
int Api::quantite_transmutation_catalyseur_or(int taille_region)
{
    // Strict check, in case the values are defined explicitly
    if (taille_region <= 0 || taille_region > TAILLE_ETABLI * TAILLE_ETABLI)
        return 0;

    return game_state_->transmute_catalyst_scoreval(taille_region);
}

/// Indique l’échantillon par défaut lors du premier tour
echantillon Api::echantillon_defaut_premier_tour()
{
    return game_state_->default_sample;
}

/// Affiche l'état actuel des deux établis dans la console.
void Api::afficher_etablis()
{
    print_workbenches(game_state(), std::cout);
}
