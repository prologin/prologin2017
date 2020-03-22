/// This file has been generated, if you wish to
/// modify it in a permanent way, please refer
/// to the script file : gen/generator_python.rb

#ifndef INTERFACE_HH_
# define INTERFACE_HH_

# include <Python.h>
# include <vector>
# include <string>

/// Types de cases
typedef enum case_type {
  VIDE, /* <- Case vide */
  PLOMB, /* <- Plomb ; transmutable en or */
  FER, /* <- Fer ; transmutable en or */
  CUIVRE, /* <- Cuivre ; transmutable en or */
  SOUFRE, /* <- Soufre ; transmutable en catalyseur */
  MERCURE, /* <- Mercure ; transmutable en catalyseur */
} case_type;
// This is needed for old compilers
namespace std
{
  template <> struct hash<case_type> {
    size_t operator()(const case_type& v) const {
      return hash<int>()(static_cast<int>(v));
    }
  };
}

/// Types de propriétés des éléments
typedef enum element_propriete {
  AUCUNE, /* <- Les cases vides ne contiennent pas d’élément, et n’ont donc aucune propriété */
  TRANSMUTABLE_OR, /* <- Élement transmutable en or */
  TRANSMUTABLE_CATALYSEUR, /* <- Élément transmutable en catalyseur */
} element_propriete;
// This is needed for old compilers
namespace std
{
  template <> struct hash<element_propriete> {
    size_t operator()(const element_propriete& v) const {
      return hash<int>()(static_cast<int>(v));
    }
  };
}

/// Erreurs possibles
typedef enum erreur {
  OK, /* <- L’action a été exécutée avec succès */
  POSITION_INVALIDE, /* <- La position spécifiée n’est pas sur l’établi */
  PLACEMENT_INVALIDE, /* <- Les deux positions ne correspondent pas à des cases adjacentes */
  PLACEMENT_IMPOSSIBLE, /* <- Les cases ciblées ne sont pas vides */
  PLACEMENT_INCORRECT, /* <- Un des deux éléments de l'échantillon doit être placé adjacent à un élément du même type déjà présent sur l'établi */
  CASE_VIDE, /* <- La case ciblée est vide */
  ECHANTILLON_INCOMPLET, /* <- L’échantillon doit contenir deux éléments. */
  ECHANTILLON_INVALIDE, /* <- L’échantillon doit contenir au moins un des éléments de l’échantillon reçu auparavant */
  AUCUN_CATALYSEUR, /* <- Aucun catalyseur disponible */
  CATALYSE_INVALIDE, /* <- L'élément de destination ne peut pas être vide. */
  DEJA_POSE, /* <- L’échantillon a déjà été posé ce tour-ci */
  DEJA_DONNE, /* <- L’échantillon a déjà été donné ce tour-ci */
} erreur;
// This is needed for old compilers
namespace std
{
  template <> struct hash<erreur> {
    size_t operator()(const erreur& v) const {
      return hash<int>()(static_cast<int>(v));
    }
  };
}

/// Types d’actions
typedef enum action_type {
  ACTION_PLACER, /* <- Action ``placer_echantillon`` */
  ACTION_TRANSMUTER, /* <- Action ``transmuter`` */
  ACTION_CATALYSER, /* <- Action ``catalyser`` */
  ACTION_DONNER_ECHANTILLON, /* <- Action ``donner_echantillon`` */
} action_type;
// This is needed for old compilers
namespace std
{
  template <> struct hash<action_type> {
    size_t operator()(const action_type& v) const {
      return hash<int>()(static_cast<int>(v));
    }
  };
}

/// Position sur la carte, donnée par deux coordonnées
typedef struct position {
  int ligne;  /* <- Coordonnée : ligne */
  int colonne;  /* <- Coordonnée : colonne */
} position;


/// Échantillon, défini par deux types d’éléments
typedef struct echantillon {
  case_type element1;  /* <- Élément 1 */
  case_type element2;  /* <- Élément 2 */
} echantillon;


/// Position d’un échantillon, donnée par deux positions adjacentes
typedef struct position_echantillon {
  position pos1;  /* <- Position de l’élément 1 de l’échantillon */
  position pos2;  /* <- Position de l’élément 2 de l’échantillon */
} position_echantillon;


/// Action représentée dans l’historique. L’action ``placer_echantillon`` utilise ``poshist1`` et ``poshist2``. L’action ``transmuter`` utilise ``poshist1``. L’action ``catalyser`` utilise ``poshist1``, ``id_apprenti`` et ``nouvelle_case``. L’action ``donner_echantillon`` n’est pas représentée dans l’historique, car ``echantillon_tour`` donne l’information.
typedef struct action_hist {
  action_type atype;  /* <- Type de l’action */
  position poshist1;  /* <- Position, pour les actions placer (1er élément), transmuter et catalyser */
  position poshist2;  /* <- Position, pour l’action placer (2e élément) */
  int id_apprenti;  /* <- ID de l’apprenti, pour l’action catalyser */
  case_type nouvelle_case;  /* <- Élément pour l’action catalyser */
} action_hist;



extern "C" {

/// Place l’échantillon du tour sur l’établi, avec les coordonnées de deux cases adjacentes.
erreur api_placer_echantillon(position pos1, position pos2);

/// Provoque la transformation chimique de l’élément à la case ciblée, ainsi que tous les éléments adjacents du même type, ceux du même type adjacents à ces derniers, etc. Ils disparaissent alors tous dans leur transmutation en or ou en catalyseur.
erreur api_transmuter(position pos);

/// Utilise un catalyseur sur la case ciblée de l'apprenti indiqué. Transforme l’ancien élément en l’élément indiqué.
erreur api_catalyser(position pos, int id_apprenti, case_type terrain);

/// Définit l’échantillon que l’adversaire recevra à son prochain tour.
erreur api_donner_echantillon(echantillon echantillon_donne);

/// Renvoie le type d’une case donnée, ou 0 si la case est invaide.
case_type api_type_case(position pos, int id_apprenti);

/// Indique si une case donnée est vide ou contient un élément. Renvoie faux en cas d'erreur.
bool api_est_vide(position pos, int id_apprenti);

/// Renvoie la propriété de l’élément sur une case donnée. Un élément invalide n'a pas de propriété.
element_propriete api_propriete_case(position pos, int id_apprenti);

/// Renvoie la propriété d’un type de case donné.
element_propriete api_propriete_case_type(case_type ctype);

/// Renvoie la taille de la région à laquelle appartient un élément. Renvoie -1 si la position est invalide.
int api_taille_region(position pos, int id_apprenti);

/// Renvoie la liste des positions des cases composant la région à laquelle appartient un élément donné. Renvoie une liste vide en cas d'erreur.
std::vector<position> api_positions_region(position pos, int id_apprenti);

/// Détermine si le placement d’un échantillon est valide.
bool api_placement_possible_echantillon(echantillon echantillon_a_placer, position pos1, position pos2, int id_apprenti);

/// Renvoie la liste des placements possibles pour un échantillon donné sur l’établi d’un apprenti donné. Renvoie une liste vide en cas d'erreur.
std::vector<position_echantillon> api_placements_possible_echantillon(echantillon echantillon_a_placer, int id_apprenti);

/// Renvoie la liste des actions jouées par l’adversaire pendant son tour, dans l’ordre chronologique.
std::vector<action_hist> api_historique();

/// Renvoie votre numéro d’apprenti.
int api_moi();

/// Renvoie le numéro d’apprenti de votre adversaire.
int api_adversaire();

/// Renvoie la quantité d’or amassée par l’apprenti désigné par le numéro ``id_apprenti``. Renvoie 0 si ``id_apprenti`` est invalide (attention, le score d’un apprenti valide peut aussi être 0).
int api_score(int id_apprenti);

/// Renvoie le numéro du tour actuel.
int api_tour_actuel();

/// Annule la dernière action. Renvoie ``false`` quand il n’y a pas d’action à annuler ce tour-ci.
bool api_annuler();

/// Indique le nombre de catalyseurs en votre possession.
int api_nombre_catalyseurs();

/// Indique l’échantillon reçu pour ce tour.
echantillon api_echantillon_tour();

/// Indique si l’échantillon reçu pour ce tour a déjà été posé.
bool api_a_pose_echantillon();

/// Indique si un échantillon a déjà été donné ce tour.
bool api_a_donne_echantillon();

/// Renvoie la quantité d’or (et donc le score) obtenue par la transmutation de ``taille_region`` éléments transmutables en or.
int api_quantite_transmutation_or(int taille_region);

/// Renvoie la quantité de catalyseurs obtenue par la transmutation de ``taille_region`` éléments transmutables en catalyseur.
int api_quantite_transmutation_catalyseur(int taille_region);

/// Renvoie la quantité d’or obtenue par la transmutation de ``taille_region`` éléments transmutables en catalyseur.
int api_quantite_transmutation_catalyseur_or(int taille_region);

/// Indique l’échantillon par défaut lors du premier tour
echantillon api_echantillon_defaut_premier_tour();

/// Affiche l'état actuel des deux établis dans la console.
void api_afficher_etablis();

/// Affiche le contenu d'une valeur de type case_type
void api_afficher_case_type(case_type v);

/// Affiche le contenu d'une valeur de type element_propriete
void api_afficher_element_propriete(element_propriete v);

/// Affiche le contenu d'une valeur de type erreur
void api_afficher_erreur(erreur v);

/// Affiche le contenu d'une valeur de type action_type
void api_afficher_action_type(action_type v);

/// Affiche le contenu d'une valeur de type position
void api_afficher_position(position v);

/// Affiche le contenu d'une valeur de type echantillon
void api_afficher_echantillon(echantillon v);

/// Affiche le contenu d'une valeur de type position_echantillon
void api_afficher_position_echantillon(position_echantillon v);

/// Affiche le contenu d'une valeur de type action_hist
void api_afficher_action_hist(action_hist v);

}

#endif // !INTERFACE_HH_
