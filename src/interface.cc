/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2017 Prologin
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
    case CASE_VIDE:
        return "\"case_vide\"";
    case ECHANTILLON_INCOMPLET:
        return "\"echantillon_incomplet\"";
    case ECHANTILLON_INVALIDE:
        return "\"echantillon_invalide\"";
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
    std::string pos1 = convert_to_string(in.pos1);
    std::string out = "{";
    out += "pos1:" + pos1;
    out += ", ";
    out += "pos1:" + pos1;
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

/// Renvoie le type d’une case donnée.
extern "C" case_type api_type_case(position pos, int id_apprenti)
{
    return api->type_case(pos, id_apprenti);
}

/// Indique si une case donnée est vide ou contient un élément.
extern "C" bool api_est_vide(position pos, int id_apprenti)
{
    return api->est_vide(pos, id_apprenti);
}

/// Renvoie la propriété de l’élément sur une case donnée.
extern "C" element_propriete api_propriete_case(position pos, int id_apprenti)
{
    return api->propriete_case(pos, id_apprenti);
}

/// Renvoie la propriété d’un type de case donné.
extern "C" element_propriete api_propriete_case_type(case_type type)
{
    return api->propriete_case_type(type);
}

/// Renvoie la taille de la région à laquelle appartient un élément.
extern "C" int api_taille_region(position pos, int id_apprenti)
{
    return api->taille_region(pos, id_apprenti);
}

/// Renvoie la liste des positions des cases composant la région à laquelle
/// appartient un élément donné.
extern "C" std::vector<position> api_positions_region(position pos,
                                                      int id_apprenti)
{
    return api->positions_region(pos, id_apprenti);
}

/// Renvoie la liste des placements possibles pour un échantillon donné sur
/// l’établi d’un apprenti donné
extern "C" std::vector<position_echantillon>
api_placements_possible_echantillon(echantillon echantillon_a_placer,
                                    int id_apprenti)
{
    return api->placements_possible_echantillon(echantillon_a_placer,
                                                id_apprenti);
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
    case CASE_VIDE:
        os << "CASE_VIDE";
        break;
    case ECHANTILLON_INCOMPLET:
        os << "ECHANTILLON_INCOMPLET";
        break;
    case ECHANTILLON_INVALIDE:
        os << "ECHANTILLON_INVALIDE";
        break;
    }
    return os;
}
extern "C" void api_afficher_erreur(erreur v)
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
    os << "pos1"
       << "=" << v.pos1;
    os << " }";
    return os;
}
extern "C" void api_afficher_position_echantillon(position_echantillon v)
{
    std::cerr << v << std::endl;
}
