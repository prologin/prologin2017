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

#include "api.hh"

#include <iostream>
#include <sstream>
#include <vector>

// from api.cc
extern Api* api;

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& arr)
{
    os << "[";
    typename std::vector<T>::const_iterator it;
    for (it = arr.begin(); it != arr.end(); ++it)
    {
        if (it != arr.begin())
            os << ", ";
        os << *it;
    }
    os << "]";
    return os;
}

// todo avoir un ostringstream a la place de std::string

std::string convert_to_string(int i)
{
    std::ostringstream s;
    s << i;
    std::string result = s.str();
    return result;
}
std::string convert_to_string(double i)
{
    std::ostringstream s;
    s << i;
    std::string result = s.str();
    return result;
}
std::string convert_to_string(std::string i)
{
    return i;
}
std::string convert_to_string(bool i)
{
    return i ? "true" : "false";
}
std::string convert_to_string(std::vector<int> in)
{
    if (in.size())
    {
        std::string s = "[" + convert_to_string(in[0]);
        for (int i = 1, l = in.size(); i < l; i++)
        {
            s = s + ", " + convert_to_string(in[i]);
        }
        return s + "]";
    }
    else
    {
        return "[]";
    }
}
std::string convert_to_string(case_type in)
{
    switch (in)
    {
    case VIDE:
        return "\"vide\"";
    case PLOMB:
        return "\"plomb\"";
    case FER:
        return "\"fer\"";
    case CUIVRE:
        return "\"cuivre\"";
    case SOUFRE:
        return "\"soufre\"";
    case MERCURE:
        return "\"mercure\"";
    }
    return "bad value";
}
std::string convert_to_string(std::vector<case_type> in)
{
    if (in.size())
    {
        std::string s = "[" + convert_to_string(in[0]);
        for (int i = 1, l = in.size(); i < l; i++)
        {
            s = s + ", " + convert_to_string(in[i]);
        }
        return s + "]";
    }
    else
    {
        return "[]";
    }
}
std::string convert_to_string(element_propriete in)
{
    switch (in)
    {
    case AUCUNE:
        return "\"aucune\"";
    case TRANSMUTABLE_OR:
        return "\"transmutable_or\"";
    case TRANSMUTABLE_CATALYSEUR:
        return "\"transmutable_catalyseur\"";
    }
    return "bad value";
}
std::string convert_to_string(std::vector<element_propriete> in)
{
    if (in.size())
    {
        std::string s = "[" + convert_to_string(in[0]);
        for (int i = 1, l = in.size(); i < l; i++)
        {
            s = s + ", " + convert_to_string(in[i]);
        }
        return s + "]";
    }
    else
    {
        return "[]";
    }
}
std::string convert_to_string(erreur in)
{
    switch (in)
    {
    case OK:
        return "\"ok\"";
    case POSITION_INVALIDE:
        return "\"position_invalide\"";
    case PLACEMENT_INVALIDE:
        return "\"placement_invalide\"";
    case PLACEMENT_IMPOSSIBLE:
        return "\"placement_impossible\"";
    case PLACEMENT_INCORRECT:
        return "\"placement_incorrect\"";
    case CASE_VIDE:
        return "\"case_vide\"";
    case ECHANTILLON_INCOMPLET:
        return "\"echantillon_incomplet\"";
    case ECHANTILLON_INVALIDE:
        return "\"echantillon_invalide\"";
    case AUCUN_CATALYSEUR:
        return "\"aucun_catalyseur\"";
    case CATALYSE_INVALIDE:
        return "\"catalyse_invalide\"";
    case DEJA_POSE:
        return "\"deja_pose\"";
    case DEJA_DONNE:
        return "\"deja_donne\"";
    }
    return "bad value";
}
std::string convert_to_string(std::vector<erreur> in)
{
    if (in.size())
    {
        std::string s = "[" + convert_to_string(in[0]);
        for (int i = 1, l = in.size(); i < l; i++)
        {
            s = s + ", " + convert_to_string(in[i]);
        }
        return s + "]";
    }
    else
    {
        return "[]";
    }
}
std::string convert_to_string(action_type in)
{
    switch (in)
    {
    case ACTION_PLACER:
        return "\"action_placer\"";
    case ACTION_TRANSMUTER:
        return "\"action_transmuter\"";
    case ACTION_CATALYSER:
        return "\"action_catalyser\"";
    case ACTION_DONNER_ECHANTILLON:
        return "\"action_donner_echantillon\"";
    }
    return "bad value";
}
std::string convert_to_string(std::vector<action_type> in)
{
    if (in.size())
    {
        std::string s = "[" + convert_to_string(in[0]);
        for (int i = 1, l = in.size(); i < l; i++)
        {
            s = s + ", " + convert_to_string(in[i]);
        }
        return s + "]";
    }
    else
    {
        return "[]";
    }
}
std::string convert_to_string(position in)
{
    std::string ligne = convert_to_string(in.ligne);
    std::string colonne = convert_to_string(in.colonne);
    std::string out = "{";
    out += "ligne:" + ligne;
    out += ", ";
    out += "colonne:" + colonne;
    return out + "}";
}

std::string convert_to_string(std::vector<position> in)
{
    if (in.size())
    {
        std::string s = "[" + convert_to_string(in[0]);
        for (int i = 1, l = in.size(); i < l; i++)
        {
            s = s + ", " + convert_to_string(in[i]);
        }
        return s + "]";
    }
    else
    {
        return "[]";
    }
}
std::string convert_to_string(echantillon in)
{
    std::string element1 = convert_to_string(in.element1);
    std::string element2 = convert_to_string(in.element2);
    std::string out = "{";
    out += "element1:" + element1;
    out += ", ";
    out += "element2:" + element2;
    return out + "}";
}

std::string convert_to_string(std::vector<echantillon> in)
{
    if (in.size())
    {
        std::string s = "[" + convert_to_string(in[0]);
        for (int i = 1, l = in.size(); i < l; i++)
        {
            s = s + ", " + convert_to_string(in[i]);
        }
        return s + "]";
    }
    else
    {
        return "[]";
    }
}
std::string convert_to_string(position_echantillon in)
{
    std::string pos1 = convert_to_string(in.pos1);
    std::string pos2 = convert_to_string(in.pos2);
    std::string out = "{";
    out += "pos1:" + pos1;
    out += ", ";
    out += "pos2:" + pos2;
    return out + "}";
}

std::string convert_to_string(std::vector<position_echantillon> in)
{
    if (in.size())
    {
        std::string s = "[" + convert_to_string(in[0]);
        for (int i = 1, l = in.size(); i < l; i++)
        {
            s = s + ", " + convert_to_string(in[i]);
        }
        return s + "]";
    }
    else
    {
        return "[]";
    }
}
std::string convert_to_string(action_hist in)
{
    std::string atype = convert_to_string(in.atype);
    std::string pos1 = convert_to_string(in.pos1);
    std::string pos2 = convert_to_string(in.pos2);
    std::string id_apprenti = convert_to_string(in.id_apprenti);
    std::string nouvelle_case = convert_to_string(in.nouvelle_case);
    std::string out = "{";
    out += "atype:" + atype;
    out += ", ";
    out += "pos1:" + pos1;
    out += ", ";
    out += "pos2:" + pos2;
    out += ", ";
    out += "id_apprenti:" + id_apprenti;
    out += ", ";
    out += "nouvelle_case:" + nouvelle_case;
    return out + "}";
}

std::string convert_to_string(std::vector<action_hist> in)
{
    if (in.size())
    {
        std::string s = "[" + convert_to_string(in[0]);
        for (int i = 1, l = in.size(); i < l; i++)
        {
            s = s + ", " + convert_to_string(in[i]);
        }
        return s + "]";
    }
    else
    {
        return "[]";
    }
}
/// Place l’échantillon du tour sur l’établi, avec les coordonnées de deux cases
/// adjacentes.
extern "C" erreur api_placer_echantillon(position pos1, position pos2)
{
    return api->placer_echantillon(pos1, pos2);
}

/// Provoque la transformation chimique de l’élément à la case ciblée, ainsi que
/// tous les éléments adjacents du même type, ceux du même type adjacents à ces
/// derniers, etc. Ils disparaissent alors tous dans leur transmutation en or ou
/// en catalyseur.
extern "C" erreur api_transmuter(position pos)
{
    return api->transmuter(pos);
}

/// Utilise un catalyseur sur la case ciblée de l'apprenti indiqué. Transforme
/// l’ancien élément en l’élément indiqué.
extern "C" erreur api_catalyser(position pos, int id_apprenti,
                                case_type terrain)
{
    return api->catalyser(pos, id_apprenti, terrain);
}

/// Définit l’échantillon que l’adversaire recevra à son prochain tour.
extern "C" erreur api_donner_echantillon(echantillon echantillon_donne)
{
    return api->donner_echantillon(echantillon_donne);
}

/// Renvoie le type d’une case donnée, ou 0 si la case est invaide.
extern "C" case_type api_type_case(position pos, int id_apprenti)
{
    return api->type_case(pos, id_apprenti);
}

/// Indique si une case donnée est vide ou contient un élément. Renvoie faux en
/// cas d'erreur.
extern "C" bool api_est_vide(position pos, int id_apprenti)
{
    return api->est_vide(pos, id_apprenti);
}

/// Renvoie la propriété de l’élément sur une case donnée. Un élément invalide
/// n'a pas de propriété.
extern "C" element_propriete api_propriete_case(position pos, int id_apprenti)
{
    return api->propriete_case(pos, id_apprenti);
}

/// Renvoie la propriété d’un type de case donné.
extern "C" element_propriete api_propriete_case_type(case_type ctype)
{
    return api->propriete_case_type(ctype);
}

/// Renvoie la taille de la région à laquelle appartient un élément. Renvoie -1
/// si la position est invalide.
extern "C" int api_taille_region(position pos, int id_apprenti)
{
    return api->taille_region(pos, id_apprenti);
}

/// Renvoie la liste des positions des cases composant la région à laquelle
/// appartient un élément donné. Renvoie une liste vide en cas d'erreur.
extern "C" std::vector<position> api_positions_region(position pos,
                                                      int id_apprenti)
{
    return api->positions_region(pos, id_apprenti);
}

/// Renvoie la liste des placements possibles pour un échantillon donné sur
/// l’établi d’un apprenti donné. Renvoie une liste vide en cas d'erreur.
extern "C" std::vector<position_echantillon>
api_placements_possible_echantillon(echantillon echantillon_a_placer,
                                    int id_apprenti)
{
    return api->placements_possible_echantillon(echantillon_a_placer,
                                                id_apprenti);
}

/// Renvoie la liste des actions jouées par l’adversaire pendant son tour, dans
/// l’ordre chronologique.
extern "C" std::vector<action_hist> api_historique()
{
    return api->historique();
}

/// Renvoie votre numéro d’apprenti.
extern "C" int api_moi()
{
    return api->moi();
}

/// Renvoie le numéro d’apprenti de votre adversaire.
extern "C" int api_adversaire()
{
    return api->adversaire();
}

/// Renvoie la quantité d’or amassée par l’apprenti désigné par le numéro
/// ``id_apprenti``. Renvoie 0 si ``id_apprenti`` est invalide (attention, le
/// score d’un apprenti valide peut aussi être 0).
extern "C" int api_score(int id_apprenti)
{
    return api->score(id_apprenti);
}

/// Renvoie le numéro du tour actuel.
extern "C" int api_tour_actuel()
{
    return api->tour_actuel();
}

/// Annule la dernière action. Renvoie ``false`` quand il n’y a pas d’action à
/// annuler ce tour-ci.
extern "C" bool api_annuler()
{
    return api->annuler();
}

/// Indique le nombre de catalyseurs en votre possession.
extern "C" int api_nombre_catalyseurs()
{
    return api->nombre_catalyseurs();
}

/// Indique l’échantillon reçu pour ce tour.
extern "C" echantillon api_echantillon_tour()
{
    return api->echantillon_tour();
}

/// Indique si l’échantillon reçu pour ce tour a déjà été posé.
extern "C" bool api_a_pose_echantillon()
{
    return api->a_pose_echantillon();
}

/// Indique si un échantillon a déjà été donné ce tour.
extern "C" bool api_a_donne_echantillon()
{
    return api->a_donne_echantillon();
}

/// Renvoie la quantité d’or (et donc le score) obtenue par la transmutation de
/// ``taille_region`` éléments transmutables en or.
extern "C" int api_quantite_transmutation_or(int taille_region)
{
    return api->quantite_transmutation_or(taille_region);
}

/// Renvoie la quantité de catalyseurs obtenue par la transmutation de
/// ``taille_region`` éléments transmutables en catalyseur.
extern "C" int api_quantite_transmutation_catalyseur(int taille_region)
{
    return api->quantite_transmutation_catalyseur(taille_region);
}

/// Renvoie la quantité d’or obtenue par la transmutation de ``taille_region``
/// éléments transmutables en catalyseur.
extern "C" int api_quantite_transmutation_catalyseur_or(int taille_region)
{
    return api->quantite_transmutation_catalyseur_or(taille_region);
}

/// Indique l’échantillon par défaut lors du premier tour
extern "C" echantillon api_echantillon_defaut_premier_tour()
{
    return api->echantillon_defaut_premier_tour();
}

/// Affiche le contenu d'une valeur de type case_type
std::ostream& operator<<(std::ostream& os, case_type v)
{
    switch (v)
    {
    case VIDE:
        os << "VIDE";
        break;
    case PLOMB:
        os << "PLOMB";
        break;
    case FER:
        os << "FER";
        break;
    case CUIVRE:
        os << "CUIVRE";
        break;
    case SOUFRE:
        os << "SOUFRE";
        break;
    case MERCURE:
        os << "MERCURE";
        break;
    }
    return os;
}
extern "C" void api_afficher_case_type(case_type v)
{
    std::cerr << v << std::endl;
}

/// Affiche le contenu d'une valeur de type element_propriete
std::ostream& operator<<(std::ostream& os, element_propriete v)
{
    switch (v)
    {
    case AUCUNE:
        os << "AUCUNE";
        break;
    case TRANSMUTABLE_OR:
        os << "TRANSMUTABLE_OR";
        break;
    case TRANSMUTABLE_CATALYSEUR:
        os << "TRANSMUTABLE_CATALYSEUR";
        break;
    }
    return os;
}
extern "C" void api_afficher_element_propriete(element_propriete v)
{
    std::cerr << v << std::endl;
}

/// Affiche le contenu d'une valeur de type erreur
std::ostream& operator<<(std::ostream& os, erreur v)
{
    switch (v)
    {
    case OK:
        os << "OK";
        break;
    case POSITION_INVALIDE:
        os << "POSITION_INVALIDE";
        break;
    case PLACEMENT_INVALIDE:
        os << "PLACEMENT_INVALIDE";
        break;
    case PLACEMENT_IMPOSSIBLE:
        os << "PLACEMENT_IMPOSSIBLE";
        break;
    case PLACEMENT_INCORRECT:
        os << "PLACEMENT_INCORRECT";
        break;
    case CASE_VIDE:
        os << "CASE_VIDE";
        break;
    case ECHANTILLON_INCOMPLET:
        os << "ECHANTILLON_INCOMPLET";
        break;
    case ECHANTILLON_INVALIDE:
        os << "ECHANTILLON_INVALIDE";
        break;
    case AUCUN_CATALYSEUR:
        os << "AUCUN_CATALYSEUR";
        break;
    case CATALYSE_INVALIDE:
        os << "CATALYSE_INVALIDE";
        break;
    case DEJA_POSE:
        os << "DEJA_POSE";
        break;
    case DEJA_DONNE:
        os << "DEJA_DONNE";
        break;
    }
    return os;
}
extern "C" void api_afficher_erreur(erreur v)
{
    std::cerr << v << std::endl;
}

/// Affiche le contenu d'une valeur de type action_type
std::ostream& operator<<(std::ostream& os, action_type v)
{
    switch (v)
    {
    case ACTION_PLACER:
        os << "ACTION_PLACER";
        break;
    case ACTION_TRANSMUTER:
        os << "ACTION_TRANSMUTER";
        break;
    case ACTION_CATALYSER:
        os << "ACTION_CATALYSER";
        break;
    case ACTION_DONNER_ECHANTILLON:
        os << "ACTION_DONNER_ECHANTILLON";
        break;
    }
    return os;
}
extern "C" void api_afficher_action_type(action_type v)
{
    std::cerr << v << std::endl;
}

/// Affiche le contenu d'une valeur de type position
std::ostream& operator<<(std::ostream& os, position v)
{
    os << "{ ";
    os << "ligne"
       << "=" << v.ligne;
    os << ", ";
    os << "colonne"
       << "=" << v.colonne;
    os << " }";
    return os;
}
extern "C" void api_afficher_position(position v)
{
    std::cerr << v << std::endl;
}

/// Affiche le contenu d'une valeur de type echantillon
std::ostream& operator<<(std::ostream& os, echantillon v)
{
    os << "{ ";
    os << "element1"
       << "=" << v.element1;
    os << ", ";
    os << "element2"
       << "=" << v.element2;
    os << " }";
    return os;
}
extern "C" void api_afficher_echantillon(echantillon v)
{
    std::cerr << v << std::endl;
}

/// Affiche le contenu d'une valeur de type position_echantillon
std::ostream& operator<<(std::ostream& os, position_echantillon v)
{
    os << "{ ";
    os << "pos1"
       << "=" << v.pos1;
    os << ", ";
    os << "pos2"
       << "=" << v.pos2;
    os << " }";
    return os;
}
extern "C" void api_afficher_position_echantillon(position_echantillon v)
{
    std::cerr << v << std::endl;
}

/// Affiche le contenu d'une valeur de type action_hist
std::ostream& operator<<(std::ostream& os, action_hist v)
{
    os << "{ ";
    os << "atype"
       << "=" << v.atype;
    os << ", ";
    os << "pos1"
       << "=" << v.pos1;
    os << ", ";
    os << "pos2"
       << "=" << v.pos2;
    os << ", ";
    os << "id_apprenti"
       << "=" << v.id_apprenti;
    os << ", ";
    os << "nouvelle_case"
       << "=" << v.nouvelle_case;
    os << " }";
    return os;
}
extern "C" void api_afficher_action_hist(action_hist v)
{
    std::cerr << v << std::endl;
}
