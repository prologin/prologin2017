// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012 Association Prologin <association@prologin.org>
#pragma once

#include <functional> // needed for std::hash

/// Taille de l’établi de travail (longueur et largeur)
#define TAILLE_ETABLI 6

/// Nombre de tours à jouer avant la fin de l’affrontement
#define NB_TOURS 150

/// Taille de l’énumération ``case_type``
#define NB_TYPE_CASES 6

/// Types de cases
typedef enum case_type
{
    VIDE,    /* <- Case vide */
    PLOMB,   /* <- Plomb ; transmutable en or */
    FER,     /* <- Fer ; transmutable en or */
    CUIVRE,  /* <- Cuivre ; transmutable en or */
    SOUFRE,  /* <- Soufre ; transmutable en catalyseur */
    MERCURE, /* <- Mercure ; transmutable en catalyseur */
} case_type;
// This is needed for old compilers
namespace std
{
template <> struct hash<case_type>
{
    size_t operator()(const case_type& v) const
    {
        return hash<int>()(static_cast<int>(v));
    }
};
} // namespace std

/// Types de propriétés des éléments
typedef enum element_propriete
{
    AUCUNE, /* <- Les cases vides ne contiennent pas d’élément, et n’ont donc
               aucune propriété */
    TRANSMUTABLE_OR,         /* <- Élement transmutable en or */
    TRANSMUTABLE_CATALYSEUR, /* <- Élément transmutable en catalyseur */
} element_propriete;
// This is needed for old compilers
namespace std
{
template <> struct hash<element_propriete>
{
    size_t operator()(const element_propriete& v) const
    {
        return hash<int>()(static_cast<int>(v));
    }
};
} // namespace std

/// Erreurs possibles
typedef enum erreur
{
    OK,                /* <- L’action a été exécutée avec succès */
    POSITION_INVALIDE, /* <- La position spécifiée n’est pas sur l’établi */
    PLACEMENT_INVALIDE,   /* <- Les deux positions ne correspondent pas à des
                             cases adjacentes */
    PLACEMENT_IMPOSSIBLE, /* <- Les cases ciblées ne sont pas vides */
    PLACEMENT_INCORRECT, /* <- Un des deux éléments de l'échantillon doit être
                            placé adjacent à un élément du même type déjà
                            présent sur l'établi */
    CASE_VIDE,           /* <- La case ciblée est vide */
    ECHANTILLON_INCOMPLET, /* <- L’échantillon doit contenir deux éléments. */
    ECHANTILLON_INVALIDE, /* <- L’échantillon doit contenir au moins un des
                             éléments de l’échantillon reçu auparavant */
    AUCUN_CATALYSEUR,     /* <- Aucun catalyseur disponible */
    CATALYSE_INVALIDE, /* <- L'élément de destination ne peut pas être vide. */
    DEJA_POSE,         /* <- L’échantillon a déjà été posé ce tour-ci */
    DEJA_DONNE, /* <- L’échantillon a déjà été donné ce tour-ci */
} erreur;
// This is needed for old compilers
namespace std
{
template <> struct hash<erreur>
{
    size_t operator()(const erreur& v) const
    {
        return hash<int>()(static_cast<int>(v));
    }
};
} // namespace std

/// Types d’actions
typedef enum action_type
{
    ACTION_PLACER,             /* <- Action ``placer_echantillon`` */
    ACTION_TRANSMUTER,         /* <- Action ``transmuter`` */
    ACTION_CATALYSER,          /* <- Action ``catalyser`` */
    ACTION_DONNER_ECHANTILLON, /* <- Action ``donner_echantillon`` */
} action_type;
// This is needed for old compilers
namespace std
{
template <> struct hash<action_type>
{
    size_t operator()(const action_type& v) const
    {
        return hash<int>()(static_cast<int>(v));
    }
};
} // namespace std

/// Position sur la carte, donnée par deux coordonnées
typedef struct position
{
    int ligne;   /* <- Coordonnée : ligne */
    int colonne; /* <- Coordonnée : colonne */
} position;

/// Échantillon, défini par deux types d’éléments
typedef struct echantillon
{
    case_type element1; /* <- Élément 1 */
    case_type element2; /* <- Élément 2 */
} echantillon;

/// Position d’un échantillon, donnée par deux positions adjacentes
typedef struct position_echantillon
{
    position pos1; /* <- Position de l’élément 1 de l’échantillon */
    position pos2; /* <- Position de l’élément 2 de l’échantillon */
} position_echantillon;

/// Action représentée dans l’historique. L’action ``placer_echantillon``
/// utilise ``poshist1`` et ``poshist2``. L’action ``transmuter`` utilise
/// ``poshist1``. L’action ``catalyser`` utilise ``poshist1``, ``id_apprenti``
/// et ``nouvelle_case``. L’action ``donner_echantillon`` n’est pas représentée
/// dans l’historique, car ``echantillon_tour`` donne l’information.
typedef struct action_hist
{
    action_type atype; /* <- Type de l’action */
    position poshist1; /* <- Position, pour les actions placer (1er élément),
                          transmuter et catalyser */
    position poshist2; /* <- Position, pour l’action placer (2e élément) */
    int id_apprenti;   /* <- ID de l’apprenti, pour l’action catalyser */
    case_type nouvelle_case; /* <- Élément pour l’action catalyser */
} action_hist;
