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

#ifndef API_HH_
#define API_HH_

#include <vector>

#include <rules/actions.hh>
#include <rules/api.hh>
#include <rules/game-state.hh>
#include <rules/player.hh>

#include "actions.hh"
#include "constant.hh"
#include "game_state.hh"

/*!
** The methods of this class are exported through 'interface.cc'
** to be called by the clients
*/
class Api final : public rules::Api<GameState, erreur>
{
public:
    Api(std::unique_ptr<GameState> game_state, rules::Player_sptr player);

    /// Place l’échantillon du tour sur l’établi, avec les coordonnées de deux
    /// cases adjacentes.
    ApiActionFunc<ActionPlacerEchantillon> placer_echantillon{this};

    /// Provoque la transformation chimique de l’élément à la case ciblée, ainsi
    /// que tous les éléments adjacents du même type, ceux du même type
    /// adjacents à ces derniers, etc. Ils disparaissent alors tous dans leur
    /// transmutation en or ou en catalyseur.
    ApiActionFunc<ActionTransmuter> transmuter{this};

    /// Utilise un catalyseur sur la case ciblée de l'apprenti indiqué.
    /// Transforme l’ancien élément en l’élément indiqué.
    ApiActionFunc<ActionCatalyser> catalyser{this};

    /// Définit l’échantillon que l’adversaire recevra à son prochain tour.
    ApiActionFunc<ActionDonnerEchantillon> donner_echantillon{this};

    /// Renvoie le type d’une case donnée, ou 0 si la case est invaide.
    case_type type_case(position pos, int id_apprenti);

    /// Indique si une case donnée est vide ou contient un élément. Renvoie faux
    /// en cas d'erreur.
    bool est_vide(position pos, int id_apprenti);

    /// Renvoie la propriété de l’élément sur une case donnée. Un élément
    /// invalide n'a pas de propriété.
    element_propriete propriete_case(position pos, int id_apprenti);

    /// Renvoie la propriété d’un type de case donné.
    element_propriete propriete_case_type(case_type ctype);

    /// Renvoie la taille de la région à laquelle appartient un élément. Renvoie
    /// -1 si la position est invalide.
    int taille_region(position pos, int id_apprenti);

    /// Renvoie la liste des positions des cases composant la région à laquelle
    /// appartient un élément donné. Renvoie une liste vide en cas d'erreur.
    std::vector<position> positions_region(position pos, int id_apprenti);

    /// Détermine si le placement d’un échantillon est valide.
    bool placement_possible_echantillon(echantillon echantillon_a_placer,
                                        position pos1, position pos2,
                                        int id_apprenti);

    /// Renvoie la liste des placements possibles pour un échantillon donné sur
    /// l’établi d’un apprenti donné. Renvoie une liste vide en cas d'erreur.
    std::vector<position_echantillon>
    placements_possible_echantillon(echantillon echantillon_a_placer,
                                    int id_apprenti);

    /// Renvoie la liste des actions jouées par l’adversaire pendant son tour,
    /// dans l’ordre chronologique.
    std::vector<action_hist> historique();

    /// Renvoie votre numéro d’apprenti.
    int moi();

    /// Renvoie le numéro d’apprenti de votre adversaire.
    int adversaire();

    /// Renvoie la quantité d’or amassée par l’apprenti désigné par le numéro
    /// ``id_apprenti``. Renvoie 0 si ``id_apprenti`` est invalide (attention,
    /// le score d’un apprenti valide peut aussi être 0).
    int score(int id_apprenti);

    /// Renvoie le numéro du tour actuel.
    int tour_actuel();

    /// Annule la dernière action. Renvoie ``false`` quand il n’y a pas d’action
    /// à annuler ce tour-ci.
    bool annuler();

    /// Indique le nombre de catalyseurs en votre possession.
    int nombre_catalyseurs();

    /// Indique l’échantillon reçu pour ce tour.
    echantillon echantillon_tour();

    /// Indique si l’échantillon reçu pour ce tour a déjà été posé.
    bool a_pose_echantillon();

    /// Indique si un échantillon a déjà été donné ce tour.
    bool a_donne_echantillon();

    /// Renvoie la quantité d’or (et donc le score) obtenue par la transmutation
    /// de ``taille_region`` éléments transmutables en or.
    int quantite_transmutation_or(int taille_region);

    /// Renvoie la quantité de catalyseurs obtenue par la transmutation de
    /// ``taille_region`` éléments transmutables en catalyseur.
    int quantite_transmutation_catalyseur(int taille_region);

    /// Renvoie la quantité d’or obtenue par la transmutation de
    /// ``taille_region`` éléments transmutables en catalyseur.
    int quantite_transmutation_catalyseur_or(int taille_region);

    /// Indique l’échantillon par défaut lors du premier tour
    echantillon echantillon_defaut_premier_tour();

    /// Affiche l'état actuel des deux établis dans la console.
    void afficher_etablis();
};

#endif /* !API_HH_ */
