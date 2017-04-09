# coding: iso-8859-1
from _api import *

# Taille de l’établi de travail (longueur et largeur)
TAILLE_ETABLI = 6

# Nombre de tours à jouer avant la fin de l’affrontement
NB_TOURS = 100

# Taille de l’énumération ``case_type``
NB_TYPE_CASES = 6


from enum import IntEnum

# Types de cases
class case_type(IntEnum):
    VIDE = 0  # <- Case vide
    PLOMB = 1  # <- Plomb ; transmutable en or
    FER = 2  # <- Fer ; transmutable en or
    CUIVRE = 3  # <- Cuivre ; transmutable en or
    SOUFRE = 4  # <- Soufre ; transmutable en catalyseur
    MERCURE = 5  # <- Mercure ; transmutable en catalyseur


# Types de propriétés des éléments
class element_propriete(IntEnum):
    AUCUNE = 0  # <- Les cases vides ne contiennent pas d’élément, et n’ont donc aucune propriété
    TRANSMUTABLE_OR = 1  # <- Élement transmutable en or
    TRANSMUTABLE_CATALYSEUR = 2  # <- Élément transmutable en catalyseur


# Erreurs possibles
class erreur(IntEnum):
    OK = 0  # <- L’action a été exécutée avec succès
    POSITION_INVALIDE = 1  # <- La position spécifiée n’est pas sur l’établi
    PLACEMENT_INVALIDE = 2  # <- Les deux positions ne correspondent pas à des cases adjacentes
    PLACEMENT_IMPOSSIBLE = 3  # <- Les cases ciblées ne sont pas vides
    PLACEMENT_INCORRECT = 4  # <- Un des deux éléments de l'échantillon doit être placé adjacent à un élément du même type déjà présent sur l'établi
    CASE_VIDE = 5  # <- La case ciblée est vide
    ECHANTILLON_INCOMPLET = 6  # <- L’échantillon doit contenir deux éléments.
    ECHANTILLON_INVALIDE = 7  # <- L’échantillon doit contenir au moins un des éléments de l’échantillon reçu auparavant
    AUCUN_CATALYSEUR = 8  # <- Aucun catalyseur disponible
    CATALYSE_INVALIDE = 9  # <- L'élément de destination ne peut pas être vide.
    DEJA_POSE = 10  # <- L’échantillon a déjà été posé ce tour-ci
    DEJA_DONNE = 11  # <- L’échantillon a déjà été donné ce tour-ci


# Types d’actions
class action_type(IntEnum):
    ACTION_PLACER = 0  # <- Action ``placer_echantillon``
    ACTION_TRANSMUTER = 1  # <- Action ``transmuter``
    ACTION_CATALYSER = 2  # <- Action ``catalyser``
    ACTION_DONNER_ECHANTILLON = 3  # <- Action ``donner_echantillon``


from collections import namedtuple

# Position sur la carte, donnée par deux coordonnées

# Échantillon, défini par deux types d’éléments

# Position d’un échantillon, donnée par deux positions adjacentes

# Action représentée dans l’historique. L’action ``placer_echantillon`` utilise ``pos1`` et ``pos2``. L’action ``transmuter`` utilise ``pos1``. L’action ``catalyser`` utilise ``pos1``, ``id_apprenti`` et ``nouvelle_case``. L’action ``donner_echantillon`` n’est pas représentée dans l’historique, car ``echantillon_tour`` donne l’information.
action_hist = namedtuple("action_hist",
    'atype ' # <- Type de l’action
    'pos1 ' # <- Position, pour les actions placer (1er élément), transmuter et catalyser
    'pos2 ' # <- Position, pour l’action placer (2e élément)
    'id_apprenti ' # <- ID de l’apprenti, pour l’action catalyser
    'nouvelle_case ' # <- Élément pour l’action catalyser
)


