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

#ifndef CONSTANT_HH_
#define CONSTANT_HH_

/// Taille de l’établi de travail (longueur et largeur)
#define TAILLE_ETABLI 6

/// Nombre de tours à jouer avant la fin de l’affrontement
#define NB_TOURS 100

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

/// Types de propriétés des éléments
typedef enum element_propriete
{
    AUCUNE, /* <- Les cases vides ne contiennent pas d’élément, et n’ont donc
               aucune propriété */
    TRANSMUTABLE_OR,         /* <- Élement transmutable en or */
    TRANSMUTABLE_CATALYSEUR, /* <- Élément transmutable en catalyseur */
} element_propriete;

/// Erreurs possibles
typedef enum erreur
{
    OK,                /* <- L’action a été exécutée avec succès */
    POSITION_INVALIDE, /* <- La position spécifiée n’est pas sur l’établi */
    PLACEMENT_INVALIDE,   /* <- Les deux positions ne correspondent pas à des
                             cases adjacentes */
    PLACEMENT_IMPOSSIBLE, /* <- Les cases ciblées ne sont pas vides */
    CASE_VIDE,            /* <- La case ciblée est vide */
    ECHANTILLON_INCOMPLET, /* <- L’échantillon doit contenir deux éléments. */
    ECHANTILLON_INVALIDE, /* <- L’échantillon doit contenir au moins un des
                             éléments de l’échantillon reçu auparavant */
} erreur;

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
    position pos1; /* <- Position de l’élément 2 de l’échantillon */
} position_echantillon;

#endif // !CONSTANT_HH_
