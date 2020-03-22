/// This file has been generated, if you wish to
/// modify it in a permanent way, please refer
/// to the script file : gen/generator_cxx.rb

#ifndef PROLOGIN_HH_
# define PROLOGIN_HH_

# include <functional>

# include <string>

# include <vector>

/// Taille de l’établi de travail (longueur et largeur)
# define TAILLE_ETABLI             6

/// Nombre de tours à jouer avant la fin de l’affrontement
# define NB_TOURS                  150

/// Taille de l’énumération ``case_type``
# define NB_TYPE_CASES             6

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


/// Place l’échantillon du tour sur l’établi, avec les coordonnées de deux cases adjacentes.
extern "C" erreur api_placer_echantillon(position pos1, position pos2);
static inline erreur placer_echantillon(position pos1, position pos2)
{
  return api_placer_echantillon(pos1, pos2);
}


/// Provoque la transformation chimique de l’élément à la case ciblée, ainsi que tous les éléments adjacents du même type, ceux du même type adjacents à ces derniers, etc. Ils disparaissent alors tous dans leur transmutation en or ou en catalyseur.
extern "C" erreur api_transmuter(position pos);
static inline erreur transmuter(position pos)
{
  return api_transmuter(pos);
}


/// Utilise un catalyseur sur la case ciblée de l'apprenti indiqué. Transforme l’ancien élément en l’élément indiqué.
extern "C" erreur api_catalyser(position pos, int id_apprenti, case_type terrain);
static inline erreur catalyser(position pos, int id_apprenti, case_type terrain)
{
  return api_catalyser(pos, id_apprenti, terrain);
}


/// Définit l’échantillon que l’adversaire recevra à son prochain tour.
extern "C" erreur api_donner_echantillon(echantillon echantillon_donne);
static inline erreur donner_echantillon(echantillon echantillon_donne)
{
  return api_donner_echantillon(echantillon_donne);
}


/// Renvoie le type d’une case donnée, ou 0 si la case est invaide.
extern "C" case_type api_type_case(position pos, int id_apprenti);
static inline case_type type_case(position pos, int id_apprenti)
{
  return api_type_case(pos, id_apprenti);
}


/// Indique si une case donnée est vide ou contient un élément. Renvoie faux en cas d'erreur.
extern "C" bool api_est_vide(position pos, int id_apprenti);
static inline bool est_vide(position pos, int id_apprenti)
{
  return api_est_vide(pos, id_apprenti);
}


/// Renvoie la propriété de l’élément sur une case donnée. Un élément invalide n'a pas de propriété.
extern "C" element_propriete api_propriete_case(position pos, int id_apprenti);
static inline element_propriete propriete_case(position pos, int id_apprenti)
{
  return api_propriete_case(pos, id_apprenti);
}


/// Renvoie la propriété d’un type de case donné.
extern "C" element_propriete api_propriete_case_type(case_type ctype);
static inline element_propriete propriete_case_type(case_type ctype)
{
  return api_propriete_case_type(ctype);
}


/// Renvoie la taille de la région à laquelle appartient un élément. Renvoie -1 si la position est invalide.
extern "C" int api_taille_region(position pos, int id_apprenti);
static inline int taille_region(position pos, int id_apprenti)
{
  return api_taille_region(pos, id_apprenti);
}


/// Renvoie la liste des positions des cases composant la région à laquelle appartient un élément donné. Renvoie une liste vide en cas d'erreur.
extern "C" std::vector<position> api_positions_region(position pos, int id_apprenti);
static inline std::vector<position> positions_region(position pos, int id_apprenti)
{
  return api_positions_region(pos, id_apprenti);
}


/// Détermine si le placement d’un échantillon est valide.
extern "C" bool api_placement_possible_echantillon(echantillon echantillon_a_placer, position pos1, position pos2, int id_apprenti);
static inline bool placement_possible_echantillon(echantillon echantillon_a_placer, position pos1, position pos2, int id_apprenti)
{
  return api_placement_possible_echantillon(echantillon_a_placer, pos1, pos2, id_apprenti);
}


/// Renvoie la liste des placements possibles pour un échantillon donné sur l’établi d’un apprenti donné. Renvoie une liste vide en cas d'erreur.
extern "C" std::vector<position_echantillon> api_placements_possible_echantillon(echantillon echantillon_a_placer, int id_apprenti);
static inline std::vector<position_echantillon> placements_possible_echantillon(echantillon echantillon_a_placer, int id_apprenti)
{
  return api_placements_possible_echantillon(echantillon_a_placer, id_apprenti);
}


/// Renvoie la liste des actions jouées par l’adversaire pendant son tour, dans l’ordre chronologique.
extern "C" std::vector<action_hist> api_historique();
static inline std::vector<action_hist> historique()
{
  return api_historique();
}


/// Renvoie votre numéro d’apprenti.
extern "C" int api_moi();
static inline int moi()
{
  return api_moi();
}


/// Renvoie le numéro d’apprenti de votre adversaire.
extern "C" int api_adversaire();
static inline int adversaire()
{
  return api_adversaire();
}


/// Renvoie la quantité d’or amassée par l’apprenti désigné par le numéro ``id_apprenti``. Renvoie 0 si ``id_apprenti`` est invalide (attention, le score d’un apprenti valide peut aussi être 0).
extern "C" int api_score(int id_apprenti);
static inline int score(int id_apprenti)
{
  return api_score(id_apprenti);
}


/// Renvoie le numéro du tour actuel.
extern "C" int api_tour_actuel();
static inline int tour_actuel()
{
  return api_tour_actuel();
}


/// Annule la dernière action. Renvoie ``false`` quand il n’y a pas d’action à annuler ce tour-ci.
extern "C" bool api_annuler();
static inline bool annuler()
{
  return api_annuler();
}


/// Indique le nombre de catalyseurs en votre possession.
extern "C" int api_nombre_catalyseurs();
static inline int nombre_catalyseurs()
{
  return api_nombre_catalyseurs();
}


/// Indique l’échantillon reçu pour ce tour.
extern "C" echantillon api_echantillon_tour();
static inline echantillon echantillon_tour()
{
  return api_echantillon_tour();
}


/// Indique si l’échantillon reçu pour ce tour a déjà été posé.
extern "C" bool api_a_pose_echantillon();
static inline bool a_pose_echantillon()
{
  return api_a_pose_echantillon();
}


/// Indique si un échantillon a déjà été donné ce tour.
extern "C" bool api_a_donne_echantillon();
static inline bool a_donne_echantillon()
{
  return api_a_donne_echantillon();
}


/// Renvoie la quantité d’or (et donc le score) obtenue par la transmutation de ``taille_region`` éléments transmutables en or.
extern "C" int api_quantite_transmutation_or(int taille_region);
static inline int quantite_transmutation_or(int taille_region)
{
  return api_quantite_transmutation_or(taille_region);
}


/// Renvoie la quantité de catalyseurs obtenue par la transmutation de ``taille_region`` éléments transmutables en catalyseur.
extern "C" int api_quantite_transmutation_catalyseur(int taille_region);
static inline int quantite_transmutation_catalyseur(int taille_region)
{
  return api_quantite_transmutation_catalyseur(taille_region);
}


/// Renvoie la quantité d’or obtenue par la transmutation de ``taille_region`` éléments transmutables en catalyseur.
extern "C" int api_quantite_transmutation_catalyseur_or(int taille_region);
static inline int quantite_transmutation_catalyseur_or(int taille_region)
{
  return api_quantite_transmutation_catalyseur_or(taille_region);
}


/// Indique l’échantillon par défaut lors du premier tour
extern "C" echantillon api_echantillon_defaut_premier_tour();
static inline echantillon echantillon_defaut_premier_tour()
{
  return api_echantillon_defaut_premier_tour();
}


/// Affiche l'état actuel des deux établis dans la console.
extern "C" void api_afficher_etablis();
static inline void afficher_etablis()
{
  api_afficher_etablis();
}


/// Affiche le contenu d'une valeur de type case_type
extern "C" void api_afficher_case_type(case_type v);
static inline void afficher_case_type(case_type v)
{
  api_afficher_case_type(v);
}


/// Affiche le contenu d'une valeur de type element_propriete
extern "C" void api_afficher_element_propriete(element_propriete v);
static inline void afficher_element_propriete(element_propriete v)
{
  api_afficher_element_propriete(v);
}


/// Affiche le contenu d'une valeur de type erreur
extern "C" void api_afficher_erreur(erreur v);
static inline void afficher_erreur(erreur v)
{
  api_afficher_erreur(v);
}


/// Affiche le contenu d'une valeur de type action_type
extern "C" void api_afficher_action_type(action_type v);
static inline void afficher_action_type(action_type v)
{
  api_afficher_action_type(v);
}


/// Affiche le contenu d'une valeur de type position
extern "C" void api_afficher_position(position v);
static inline void afficher_position(position v)
{
  api_afficher_position(v);
}


/// Affiche le contenu d'une valeur de type echantillon
extern "C" void api_afficher_echantillon(echantillon v);
static inline void afficher_echantillon(echantillon v)
{
  api_afficher_echantillon(v);
}


/// Affiche le contenu d'une valeur de type position_echantillon
extern "C" void api_afficher_position_echantillon(position_echantillon v);
static inline void afficher_position_echantillon(position_echantillon v)
{
  api_afficher_position_echantillon(v);
}


/// Affiche le contenu d'une valeur de type action_hist
extern "C" void api_afficher_action_hist(action_hist v);
static inline void afficher_action_hist(action_hist v)
{
  api_afficher_action_hist(v);
}


// Les fonctions suivantes définissent les opérations de comparaison, d'égalité
// et de hachage sur les structures du sujet.

namespace std {
  template <typename T>
  struct hash<std::vector<T>>
  {
    std::size_t operator()(const std::vector<T>& v) const
    {
      std::size_t res = v.size();
      for (const auto& e : v)
        res ^= std::hash<T>()(e) + 0x9e3779b9 + (res << 6) + (res >> 2);
      return res;
    }
  };
}

inline bool operator==(const position& a, const position& b) {
  if (a.ligne != b.ligne) return false;
  if (a.colonne != b.colonne) return false;
  return true;
}

inline bool operator!=(const position& a, const position& b) {
  if (a.ligne != b.ligne) return true;
  if (a.colonne != b.colonne) return true;
  return false;
}

inline bool operator<(const position& a, const position& b) {
  if (a.ligne < b.ligne) return true;
  if (a.ligne > b.ligne) return false;
  if (a.colonne < b.colonne) return true;
  if (a.colonne > b.colonne) return false;
  return false;
}

inline bool operator>(const position& a, const position& b) {
  if (a.ligne > b.ligne) return true;
  if (a.ligne < b.ligne) return false;
  if (a.colonne > b.colonne) return true;
  if (a.colonne < b.colonne) return false;
  return false;
}

namespace std {
  template <>
  struct hash<position>
  {
    std::size_t operator()(const position& s) const
    {
      std::size_t res = 0;
      res ^= 0x9e3779b9 + (res << 6) + (res >> 2) + std::hash<int>()(s.ligne);
      res ^= 0x9e3779b9 + (res << 6) + (res >> 2) + std::hash<int>()(s.colonne);
      return res;
    }
  };
}

inline bool operator==(const echantillon& a, const echantillon& b) {
  if (a.element1 != b.element1) return false;
  if (a.element2 != b.element2) return false;
  return true;
}

inline bool operator!=(const echantillon& a, const echantillon& b) {
  if (a.element1 != b.element1) return true;
  if (a.element2 != b.element2) return true;
  return false;
}

inline bool operator<(const echantillon& a, const echantillon& b) {
  if (a.element1 < b.element1) return true;
  if (a.element1 > b.element1) return false;
  if (a.element2 < b.element2) return true;
  if (a.element2 > b.element2) return false;
  return false;
}

inline bool operator>(const echantillon& a, const echantillon& b) {
  if (a.element1 > b.element1) return true;
  if (a.element1 < b.element1) return false;
  if (a.element2 > b.element2) return true;
  if (a.element2 < b.element2) return false;
  return false;
}

namespace std {
  template <>
  struct hash<echantillon>
  {
    std::size_t operator()(const echantillon& s) const
    {
      std::size_t res = 0;
      res ^= 0x9e3779b9 + (res << 6) + (res >> 2) + std::hash<case_type>()(s.element1);
      res ^= 0x9e3779b9 + (res << 6) + (res >> 2) + std::hash<case_type>()(s.element2);
      return res;
    }
  };
}

inline bool operator==(const position_echantillon& a, const position_echantillon& b) {
  if (a.pos1 != b.pos1) return false;
  if (a.pos2 != b.pos2) return false;
  return true;
}

inline bool operator!=(const position_echantillon& a, const position_echantillon& b) {
  if (a.pos1 != b.pos1) return true;
  if (a.pos2 != b.pos2) return true;
  return false;
}

inline bool operator<(const position_echantillon& a, const position_echantillon& b) {
  if (a.pos1 < b.pos1) return true;
  if (a.pos1 > b.pos1) return false;
  if (a.pos2 < b.pos2) return true;
  if (a.pos2 > b.pos2) return false;
  return false;
}

inline bool operator>(const position_echantillon& a, const position_echantillon& b) {
  if (a.pos1 > b.pos1) return true;
  if (a.pos1 < b.pos1) return false;
  if (a.pos2 > b.pos2) return true;
  if (a.pos2 < b.pos2) return false;
  return false;
}

namespace std {
  template <>
  struct hash<position_echantillon>
  {
    std::size_t operator()(const position_echantillon& s) const
    {
      std::size_t res = 0;
      res ^= 0x9e3779b9 + (res << 6) + (res >> 2) + std::hash<position>()(s.pos1);
      res ^= 0x9e3779b9 + (res << 6) + (res >> 2) + std::hash<position>()(s.pos2);
      return res;
    }
  };
}

inline bool operator==(const action_hist& a, const action_hist& b) {
  if (a.atype != b.atype) return false;
  if (a.poshist1 != b.poshist1) return false;
  if (a.poshist2 != b.poshist2) return false;
  if (a.id_apprenti != b.id_apprenti) return false;
  if (a.nouvelle_case != b.nouvelle_case) return false;
  return true;
}

inline bool operator!=(const action_hist& a, const action_hist& b) {
  if (a.atype != b.atype) return true;
  if (a.poshist1 != b.poshist1) return true;
  if (a.poshist2 != b.poshist2) return true;
  if (a.id_apprenti != b.id_apprenti) return true;
  if (a.nouvelle_case != b.nouvelle_case) return true;
  return false;
}

inline bool operator<(const action_hist& a, const action_hist& b) {
  if (a.atype < b.atype) return true;
  if (a.atype > b.atype) return false;
  if (a.poshist1 < b.poshist1) return true;
  if (a.poshist1 > b.poshist1) return false;
  if (a.poshist2 < b.poshist2) return true;
  if (a.poshist2 > b.poshist2) return false;
  if (a.id_apprenti < b.id_apprenti) return true;
  if (a.id_apprenti > b.id_apprenti) return false;
  if (a.nouvelle_case < b.nouvelle_case) return true;
  if (a.nouvelle_case > b.nouvelle_case) return false;
  return false;
}

inline bool operator>(const action_hist& a, const action_hist& b) {
  if (a.atype > b.atype) return true;
  if (a.atype < b.atype) return false;
  if (a.poshist1 > b.poshist1) return true;
  if (a.poshist1 < b.poshist1) return false;
  if (a.poshist2 > b.poshist2) return true;
  if (a.poshist2 < b.poshist2) return false;
  if (a.id_apprenti > b.id_apprenti) return true;
  if (a.id_apprenti < b.id_apprenti) return false;
  if (a.nouvelle_case > b.nouvelle_case) return true;
  if (a.nouvelle_case < b.nouvelle_case) return false;
  return false;
}

namespace std {
  template <>
  struct hash<action_hist>
  {
    std::size_t operator()(const action_hist& s) const
    {
      std::size_t res = 0;
      res ^= 0x9e3779b9 + (res << 6) + (res >> 2) + std::hash<action_type>()(s.atype);
      res ^= 0x9e3779b9 + (res << 6) + (res >> 2) + std::hash<position>()(s.poshist1);
      res ^= 0x9e3779b9 + (res << 6) + (res >> 2) + std::hash<position>()(s.poshist2);
      res ^= 0x9e3779b9 + (res << 6) + (res >> 2) + std::hash<int>()(s.id_apprenti);
      res ^= 0x9e3779b9 + (res << 6) + (res >> 2) + std::hash<case_type>()(s.nouvelle_case);
      return res;
    }
  };
}


extern "C" {

/// Fonction appelée au début de la partie.
void partie_init();

/// Fonction appelée à chaque tour.
void jouer_tour();

/// Fonction appelée à la fin de la partie.
void partie_fin();

}
#endif
