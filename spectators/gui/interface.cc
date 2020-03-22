/// This file has been generated, if you wish to
/// modify it in a permanent way, please refer
/// to the script file : gen/generator_python.rb

#include "interface.hh"

static PyObject* c_module;
static PyObject* py_module;
static PyObject* champ_module;

static void _init_python();

template <typename Lang, typename Cxx>
Lang cxx2lang(Cxx in)
{
  return in.__if_that_triggers_an_error_there_is_a_problem;
}

template <>
PyObject* cxx2lang<PyObject*, int>(int in)
{
  return PyLong_FromLong(in);
}


template <>
PyObject* cxx2lang<PyObject*, double>(double in)
{
  return PyFloat_FromDouble(in);
}


template <>
PyObject* cxx2lang<PyObject*, std::string>(std::string in)
{
  return PyUnicode_FromString(in.c_str());
}


template <>
PyObject* cxx2lang<PyObject*, bool>(bool in)
{
  return PyBool_FromLong(in);
}

template <typename Cxx>
PyObject* cxx2lang_array(const std::vector<Cxx>& in)
{
  size_t size = in.size();
  PyObject* out = PyList_New(size);

  for (unsigned int i = 0; i < size; ++i)
    PyList_SET_ITEM(out, i, (cxx2lang<PyObject*, Cxx>(in[i])));

  return out;
}

template <typename Lang, typename Cxx>
Cxx lang2cxx(Lang in)
{
  return in.__if_that_triggers_an_error_there_is_a_problem;
}

template <>
int lang2cxx<PyObject*, int>(PyObject* in)
{
  long out = PyLong_AsLong(in);
  if (PyErr_Occurred())
    throw 42;
  return out;
}

template <>
double lang2cxx<PyObject*, double>(PyObject* in)
{
  double out = PyFloat_AsDouble(in);
  if (PyErr_Occurred())
    throw 42;
  return out;
}

template <>
bool lang2cxx<PyObject*, bool>(PyObject* in)
{
  return (bool)lang2cxx<PyObject*, int>(in);
}

template <>
std::string lang2cxx<PyObject*, std::string>(PyObject* in)
{
  const char * out = PyUnicode_AsUTF8(in);
  if (PyErr_Occurred())
    {
      throw 42;
    }
  return out;
}

template <typename Cxx>
std::vector<Cxx> lang2cxx_array(PyObject* in)
{
  if (!PyList_Check(in))
  {
    PyErr_SetString(PyExc_TypeError, "a list is required");
    throw 42;
  }

  std::vector<Cxx> out;
  unsigned int size = PyList_Size(in);

  for (unsigned int i = 0; i < size; ++i)
    out.push_back(lang2cxx<PyObject*, Cxx>(PyList_GET_ITEM(in, i)));

  return out;
}
/// Types de cases
template <>
PyObject* cxx2lang<PyObject*, case_type>(case_type in)
{
  PyObject* name = PyUnicode_FromString("case_type");
  PyObject* enm = PyObject_GetAttr(py_module, name);
  if (enm == NULL) throw 42;
  PyObject* arglist = Py_BuildValue("(i)", (int) in);
  PyObject* ret = PyObject_CallObject(enm, arglist);
  Py_DECREF(name);
  Py_DECREF(arglist);
  Py_DECREF(enm);
  return ret;
}

template <>
case_type lang2cxx<PyObject*, case_type>(PyObject* in)
{
  return (case_type)lang2cxx<PyObject*, int>(in);
}

/// Types de propriétés des éléments
template <>
PyObject* cxx2lang<PyObject*, element_propriete>(element_propriete in)
{
  PyObject* name = PyUnicode_FromString("element_propriete");
  PyObject* enm = PyObject_GetAttr(py_module, name);
  if (enm == NULL) throw 42;
  PyObject* arglist = Py_BuildValue("(i)", (int) in);
  PyObject* ret = PyObject_CallObject(enm, arglist);
  Py_DECREF(name);
  Py_DECREF(arglist);
  Py_DECREF(enm);
  return ret;
}

template <>
element_propriete lang2cxx<PyObject*, element_propriete>(PyObject* in)
{
  return (element_propriete)lang2cxx<PyObject*, int>(in);
}

/// Erreurs possibles
template <>
PyObject* cxx2lang<PyObject*, erreur>(erreur in)
{
  PyObject* name = PyUnicode_FromString("erreur");
  PyObject* enm = PyObject_GetAttr(py_module, name);
  if (enm == NULL) throw 42;
  PyObject* arglist = Py_BuildValue("(i)", (int) in);
  PyObject* ret = PyObject_CallObject(enm, arglist);
  Py_DECREF(name);
  Py_DECREF(arglist);
  Py_DECREF(enm);
  return ret;
}

template <>
erreur lang2cxx<PyObject*, erreur>(PyObject* in)
{
  return (erreur)lang2cxx<PyObject*, int>(in);
}

/// Types d’actions
template <>
PyObject* cxx2lang<PyObject*, action_type>(action_type in)
{
  PyObject* name = PyUnicode_FromString("action_type");
  PyObject* enm = PyObject_GetAttr(py_module, name);
  if (enm == NULL) throw 42;
  PyObject* arglist = Py_BuildValue("(i)", (int) in);
  PyObject* ret = PyObject_CallObject(enm, arglist);
  Py_DECREF(name);
  Py_DECREF(arglist);
  Py_DECREF(enm);
  return ret;
}

template <>
action_type lang2cxx<PyObject*, action_type>(PyObject* in)
{
  return (action_type)lang2cxx<PyObject*, int>(in);
}

/// Position sur la carte, donnée par deux coordonnées
template <>
PyObject* cxx2lang<PyObject*, position>(position in)
{
  PyObject* tuple = PyTuple_New(2);
  PyTuple_SET_ITEM(tuple, 0, (cxx2lang<PyObject*, int>(in.ligne)));
  PyTuple_SET_ITEM(tuple, 1, (cxx2lang<PyObject*, int>(in.colonne)));
  return tuple;
}

template <>
position lang2cxx<PyObject*, position>(PyObject* in)
{
  position out;
  PyObject* i;
  i = PyTuple_GetItem(in, 0);
  if (i == NULL) throw 42;
  out.ligne = lang2cxx<PyObject*, int>(i);
  i = PyTuple_GetItem(in, 1);
  if (i == NULL) throw 42;
  out.colonne = lang2cxx<PyObject*, int>(i);
  return out;
}

/// Échantillon, défini par deux types d’éléments
template <>
PyObject* cxx2lang<PyObject*, echantillon>(echantillon in)
{
  PyObject* tuple = PyTuple_New(2);
  PyTuple_SET_ITEM(tuple, 0, (cxx2lang<PyObject*, case_type>(in.element1)));
  PyTuple_SET_ITEM(tuple, 1, (cxx2lang<PyObject*, case_type>(in.element2)));
  return tuple;
}

template <>
echantillon lang2cxx<PyObject*, echantillon>(PyObject* in)
{
  echantillon out;
  PyObject* i;
  i = PyTuple_GetItem(in, 0);
  if (i == NULL) throw 42;
  out.element1 = lang2cxx<PyObject*, case_type>(i);
  i = PyTuple_GetItem(in, 1);
  if (i == NULL) throw 42;
  out.element2 = lang2cxx<PyObject*, case_type>(i);
  return out;
}

/// Position d’un échantillon, donnée par deux positions adjacentes
template <>
PyObject* cxx2lang<PyObject*, position_echantillon>(position_echantillon in)
{
  PyObject* tuple = PyTuple_New(2);
  PyTuple_SET_ITEM(tuple, 0, (cxx2lang<PyObject*, position>(in.pos1)));
  PyTuple_SET_ITEM(tuple, 1, (cxx2lang<PyObject*, position>(in.pos2)));
  return tuple;
}

template <>
position_echantillon lang2cxx<PyObject*, position_echantillon>(PyObject* in)
{
  position_echantillon out;
  PyObject* i;
  i = PyTuple_GetItem(in, 0);
  if (i == NULL) throw 42;
  out.pos1 = lang2cxx<PyObject*, position>(i);
  i = PyTuple_GetItem(in, 1);
  if (i == NULL) throw 42;
  out.pos2 = lang2cxx<PyObject*, position>(i);
  return out;
}

/// Action représentée dans l’historique. L’action ``placer_echantillon`` utilise ``poshist1`` et ``poshist2``. L’action ``transmuter`` utilise ``poshist1``. L’action ``catalyser`` utilise ``poshist1``, ``id_apprenti`` et ``nouvelle_case``. L’action ``donner_echantillon`` n’est pas représentée dans l’historique, car ``echantillon_tour`` donne l’information.
template <>
PyObject* cxx2lang<PyObject*, action_hist>(action_hist in)
{
  PyObject* tuple = PyTuple_New(5);
  PyTuple_SET_ITEM(tuple, 0, (cxx2lang<PyObject*, action_type>(in.atype)));
  PyTuple_SET_ITEM(tuple, 1, (cxx2lang<PyObject*, position>(in.poshist1)));
  PyTuple_SET_ITEM(tuple, 2, (cxx2lang<PyObject*, position>(in.poshist2)));
  PyTuple_SET_ITEM(tuple, 3, (cxx2lang<PyObject*, int>(in.id_apprenti)));
  PyTuple_SET_ITEM(tuple, 4, (cxx2lang<PyObject*, case_type>(in.nouvelle_case)));
  PyObject* name = PyUnicode_FromString("action_hist");
  PyObject* cstr = PyObject_GetAttr(py_module, name);
  Py_DECREF(name);
  if (cstr == NULL) throw 42;
  PyObject* ret = PyObject_CallObject(cstr, tuple);
  Py_DECREF(cstr);
  Py_DECREF(tuple);
  if (ret == NULL) throw 42;
  return ret;
}

template <>
action_hist lang2cxx<PyObject*, action_hist>(PyObject* in)
{
  action_hist out;
  PyObject* i;
  i = cxx2lang<PyObject*, int>(0);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.atype = lang2cxx<PyObject*, action_type>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(1);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.poshist1 = lang2cxx<PyObject*, position>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(2);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.poshist2 = lang2cxx<PyObject*, position>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(3);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.id_apprenti = lang2cxx<PyObject*, int>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(4);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.nouvelle_case = lang2cxx<PyObject*, case_type>(i);
  Py_DECREF(i);
  return out;
}

/// Place l’échantillon du tour sur l’établi, avec les coordonnées de deux cases adjacentes.
static PyObject* p_placer_echantillon(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
  if (!PyArg_ParseTuple(args, "OO", &a0, &a1)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_placer_echantillon(lang2cxx<PyObject*, position>(a0), lang2cxx<PyObject*, position>(a1)));
  } catch (...) { return NULL; }
}

/// Provoque la transformation chimique de l’élément à la case ciblée, ainsi que tous les éléments adjacents du même type, ceux du même type adjacents à ces derniers, etc. Ils disparaissent alors tous dans leur transmutation en or ou en catalyseur.
static PyObject* p_transmuter(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_transmuter(lang2cxx<PyObject*, position>(a0)));
  } catch (...) { return NULL; }
}

/// Utilise un catalyseur sur la case ciblée de l'apprenti indiqué. Transforme l’ancien élément en l’élément indiqué.
static PyObject* p_catalyser(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
PyObject* a2;
  if (!PyArg_ParseTuple(args, "OOO", &a0, &a1, &a2)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_catalyser(lang2cxx<PyObject*, position>(a0), lang2cxx<PyObject*, int>(a1), lang2cxx<PyObject*, case_type>(a2)));
  } catch (...) { return NULL; }
}

/// Définit l’échantillon que l’adversaire recevra à son prochain tour.
static PyObject* p_donner_echantillon(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_donner_echantillon(lang2cxx<PyObject*, echantillon>(a0)));
  } catch (...) { return NULL; }
}

/// Renvoie le type d’une case donnée, ou 0 si la case est invaide.
static PyObject* p_type_case(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
  if (!PyArg_ParseTuple(args, "OO", &a0, &a1)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, case_type>(api_type_case(lang2cxx<PyObject*, position>(a0), lang2cxx<PyObject*, int>(a1)));
  } catch (...) { return NULL; }
}

/// Indique si une case donnée est vide ou contient un élément. Renvoie faux en cas d'erreur.
static PyObject* p_est_vide(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
  if (!PyArg_ParseTuple(args, "OO", &a0, &a1)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, bool>(api_est_vide(lang2cxx<PyObject*, position>(a0), lang2cxx<PyObject*, int>(a1)));
  } catch (...) { return NULL; }
}

/// Renvoie la propriété de l’élément sur une case donnée. Un élément invalide n'a pas de propriété.
static PyObject* p_propriete_case(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
  if (!PyArg_ParseTuple(args, "OO", &a0, &a1)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, element_propriete>(api_propriete_case(lang2cxx<PyObject*, position>(a0), lang2cxx<PyObject*, int>(a1)));
  } catch (...) { return NULL; }
}

/// Renvoie la propriété d’un type de case donné.
static PyObject* p_propriete_case_type(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, element_propriete>(api_propriete_case_type(lang2cxx<PyObject*, case_type>(a0)));
  } catch (...) { return NULL; }
}

/// Renvoie la taille de la région à laquelle appartient un élément. Renvoie -1 si la position est invalide.
static PyObject* p_taille_region(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
  if (!PyArg_ParseTuple(args, "OO", &a0, &a1)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, int>(api_taille_region(lang2cxx<PyObject*, position>(a0), lang2cxx<PyObject*, int>(a1)));
  } catch (...) { return NULL; }
}

/// Renvoie la liste des positions des cases composant la région à laquelle appartient un élément donné. Renvoie une liste vide en cas d'erreur.
static PyObject* p_positions_region(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
  if (!PyArg_ParseTuple(args, "OO", &a0, &a1)) {
    return NULL;
  }
    try {
return cxx2lang_array(api_positions_region(lang2cxx<PyObject*, position>(a0), lang2cxx<PyObject*, int>(a1)));
  } catch (...) { return NULL; }
}

/// Détermine si le placement d’un échantillon est valide.
static PyObject* p_placement_possible_echantillon(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
PyObject* a2;
PyObject* a3;
  if (!PyArg_ParseTuple(args, "OOOO", &a0, &a1, &a2, &a3)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, bool>(api_placement_possible_echantillon(lang2cxx<PyObject*, echantillon>(a0), lang2cxx<PyObject*, position>(a1), lang2cxx<PyObject*, position>(a2), lang2cxx<PyObject*, int>(a3)));
  } catch (...) { return NULL; }
}

/// Renvoie la liste des placements possibles pour un échantillon donné sur l’établi d’un apprenti donné. Renvoie une liste vide en cas d'erreur.
static PyObject* p_placements_possible_echantillon(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
  if (!PyArg_ParseTuple(args, "OO", &a0, &a1)) {
    return NULL;
  }
    try {
return cxx2lang_array(api_placements_possible_echantillon(lang2cxx<PyObject*, echantillon>(a0), lang2cxx<PyObject*, int>(a1)));
  } catch (...) { return NULL; }
}

/// Renvoie la liste des actions jouées par l’adversaire pendant son tour, dans l’ordre chronologique.
static PyObject* p_historique(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang_array(api_historique());
  } catch (...) { return NULL; }
}

/// Renvoie votre numéro d’apprenti.
static PyObject* p_moi(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, int>(api_moi());
  } catch (...) { return NULL; }
}

/// Renvoie le numéro d’apprenti de votre adversaire.
static PyObject* p_adversaire(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, int>(api_adversaire());
  } catch (...) { return NULL; }
}

/// Renvoie la quantité d’or amassée par l’apprenti désigné par le numéro ``id_apprenti``. Renvoie 0 si ``id_apprenti`` est invalide (attention, le score d’un apprenti valide peut aussi être 0).
static PyObject* p_score(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, int>(api_score(lang2cxx<PyObject*, int>(a0)));
  } catch (...) { return NULL; }
}

/// Renvoie le numéro du tour actuel.
static PyObject* p_tour_actuel(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, int>(api_tour_actuel());
  } catch (...) { return NULL; }
}

/// Annule la dernière action. Renvoie ``false`` quand il n’y a pas d’action à annuler ce tour-ci.
static PyObject* p_annuler(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, bool>(api_annuler());
  } catch (...) { return NULL; }
}

/// Indique le nombre de catalyseurs en votre possession.
static PyObject* p_nombre_catalyseurs(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, int>(api_nombre_catalyseurs());
  } catch (...) { return NULL; }
}

/// Indique l’échantillon reçu pour ce tour.
static PyObject* p_echantillon_tour(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, echantillon>(api_echantillon_tour());
  } catch (...) { return NULL; }
}

/// Indique si l’échantillon reçu pour ce tour a déjà été posé.
static PyObject* p_a_pose_echantillon(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, bool>(api_a_pose_echantillon());
  } catch (...) { return NULL; }
}

/// Indique si un échantillon a déjà été donné ce tour.
static PyObject* p_a_donne_echantillon(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, bool>(api_a_donne_echantillon());
  } catch (...) { return NULL; }
}

/// Renvoie la quantité d’or (et donc le score) obtenue par la transmutation de ``taille_region`` éléments transmutables en or.
static PyObject* p_quantite_transmutation_or(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, int>(api_quantite_transmutation_or(lang2cxx<PyObject*, int>(a0)));
  } catch (...) { return NULL; }
}

/// Renvoie la quantité de catalyseurs obtenue par la transmutation de ``taille_region`` éléments transmutables en catalyseur.
static PyObject* p_quantite_transmutation_catalyseur(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, int>(api_quantite_transmutation_catalyseur(lang2cxx<PyObject*, int>(a0)));
  } catch (...) { return NULL; }
}

/// Renvoie la quantité d’or obtenue par la transmutation de ``taille_region`` éléments transmutables en catalyseur.
static PyObject* p_quantite_transmutation_catalyseur_or(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, int>(api_quantite_transmutation_catalyseur_or(lang2cxx<PyObject*, int>(a0)));
  } catch (...) { return NULL; }
}

/// Indique l’échantillon par défaut lors du premier tour
static PyObject* p_echantillon_defaut_premier_tour(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, echantillon>(api_echantillon_defaut_premier_tour());
  } catch (...) { return NULL; }
}

/// Affiche l'état actuel des deux établis dans la console.
static PyObject* p_afficher_etablis(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
api_afficher_etablis();
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}

/// Affiche le contenu d'une valeur de type case_type
static PyObject* p_afficher_case_type(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_case_type(lang2cxx<PyObject*, case_type>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}

/// Affiche le contenu d'une valeur de type element_propriete
static PyObject* p_afficher_element_propriete(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_element_propriete(lang2cxx<PyObject*, element_propriete>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}

/// Affiche le contenu d'une valeur de type erreur
static PyObject* p_afficher_erreur(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_erreur(lang2cxx<PyObject*, erreur>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}

/// Affiche le contenu d'une valeur de type action_type
static PyObject* p_afficher_action_type(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_action_type(lang2cxx<PyObject*, action_type>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}

/// Affiche le contenu d'une valeur de type position
static PyObject* p_afficher_position(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_position(lang2cxx<PyObject*, position>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}

/// Affiche le contenu d'une valeur de type echantillon
static PyObject* p_afficher_echantillon(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_echantillon(lang2cxx<PyObject*, echantillon>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}

/// Affiche le contenu d'une valeur de type position_echantillon
static PyObject* p_afficher_position_echantillon(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_position_echantillon(lang2cxx<PyObject*, position_echantillon>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}

/// Affiche le contenu d'une valeur de type action_hist
static PyObject* p_afficher_action_hist(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_action_hist(lang2cxx<PyObject*, action_hist>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}


/*
** Api functions to register.
*/
static PyMethodDef api_callback[] = {
  {"placer_echantillon", p_placer_echantillon, METH_VARARGS, "placer_echantillon"},  {"transmuter", p_transmuter, METH_VARARGS, "transmuter"},  {"catalyser", p_catalyser, METH_VARARGS, "catalyser"},  {"donner_echantillon", p_donner_echantillon, METH_VARARGS, "donner_echantillon"},  {"type_case", p_type_case, METH_VARARGS, "type_case"},  {"est_vide", p_est_vide, METH_VARARGS, "est_vide"},  {"propriete_case", p_propriete_case, METH_VARARGS, "propriete_case"},  {"propriete_case_type", p_propriete_case_type, METH_VARARGS, "propriete_case_type"},  {"taille_region", p_taille_region, METH_VARARGS, "taille_region"},  {"positions_region", p_positions_region, METH_VARARGS, "positions_region"},  {"placement_possible_echantillon", p_placement_possible_echantillon, METH_VARARGS, "placement_possible_echantillon"},  {"placements_possible_echantillon", p_placements_possible_echantillon, METH_VARARGS, "placements_possible_echantillon"},  {"historique", p_historique, METH_VARARGS, "historique"},  {"moi", p_moi, METH_VARARGS, "moi"},  {"adversaire", p_adversaire, METH_VARARGS, "adversaire"},  {"score", p_score, METH_VARARGS, "score"},  {"tour_actuel", p_tour_actuel, METH_VARARGS, "tour_actuel"},  {"annuler", p_annuler, METH_VARARGS, "annuler"},  {"nombre_catalyseurs", p_nombre_catalyseurs, METH_VARARGS, "nombre_catalyseurs"},  {"echantillon_tour", p_echantillon_tour, METH_VARARGS, "echantillon_tour"},  {"a_pose_echantillon", p_a_pose_echantillon, METH_VARARGS, "a_pose_echantillon"},  {"a_donne_echantillon", p_a_donne_echantillon, METH_VARARGS, "a_donne_echantillon"},  {"quantite_transmutation_or", p_quantite_transmutation_or, METH_VARARGS, "quantite_transmutation_or"},  {"quantite_transmutation_catalyseur", p_quantite_transmutation_catalyseur, METH_VARARGS, "quantite_transmutation_catalyseur"},  {"quantite_transmutation_catalyseur_or", p_quantite_transmutation_catalyseur_or, METH_VARARGS, "quantite_transmutation_catalyseur_or"},  {"echantillon_defaut_premier_tour", p_echantillon_defaut_premier_tour, METH_VARARGS, "echantillon_defaut_premier_tour"},  {"afficher_etablis", p_afficher_etablis, METH_VARARGS, "afficher_etablis"},  {"afficher_case_type", p_afficher_case_type, METH_VARARGS, "afficher_case_type"},  {"afficher_element_propriete", p_afficher_element_propriete, METH_VARARGS, "afficher_element_propriete"},  {"afficher_erreur", p_afficher_erreur, METH_VARARGS, "afficher_erreur"},  {"afficher_action_type", p_afficher_action_type, METH_VARARGS, "afficher_action_type"},  {"afficher_position", p_afficher_position, METH_VARARGS, "afficher_position"},  {"afficher_echantillon", p_afficher_echantillon, METH_VARARGS, "afficher_echantillon"},  {"afficher_position_echantillon", p_afficher_position_echantillon, METH_VARARGS, "afficher_position_echantillon"},  {"afficher_action_hist", p_afficher_action_hist, METH_VARARGS, "afficher_action_hist"},  {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC PyInit__api()
{
  static struct PyModuleDef apimoduledef = {
      PyModuleDef_HEAD_INIT,
      "_api",
      "API module",
      -1,
      api_callback,
      NULL,
      NULL,
      NULL,
      NULL,
  };
  return PyModule_Create(&apimoduledef);
}


/*
** Load a Python module
*/

static PyObject* _import_module(const char* m)
{
  PyObject* name = PyUnicode_FromString(m);
  PyObject* module = PyImport_Import(name);
  Py_DECREF(name);
  if (module == NULL)
    if (PyErr_Occurred())
    {
      PyErr_Print();
      abort();
    }
  return module;
}

/*
** Inititialize python, register API functions,
** and load .py file
*/
static void _init_python()
{
  static wchar_t empty_string[] = L"";
  static wchar_t *argv[] = { (wchar_t *) &empty_string, NULL };

  const char* champion_path;

  champion_path = getenv("CHAMPION_PATH");
  if (champion_path == NULL)
    champion_path = ".";

  setenv("PYTHONPATH", champion_path, 1);

  static wchar_t program_name[] = L"stechec";
  Py_SetProgramName(program_name);

  PyImport_AppendInittab("_api", PyInit__api);
  Py_Initialize();
  PySys_SetArgvEx(1, argv, 0);

  champ_module = _import_module("prologin");
  py_module = _import_module("api");
}

/*
** Run a python function.
*/
static PyObject* _call_python_function(const char* name)
{
  static bool initialized = false;

  if (!initialized)
  {
    initialized = true;
    _init_python();
  }

  PyObject *arglist, *func;
  PyObject *result = NULL;

  func = PyObject_GetAttrString(champ_module, (char*)name);
  if (func && PyCallable_Check(func))
  {
    arglist = Py_BuildValue("()");
    result = PyEval_CallObject(func, arglist);
    Py_XDECREF(arglist);
    Py_DECREF(func);
  }
  if (result == NULL && PyErr_Occurred())
    PyErr_Print();

  return result;
}

/*
** Functions called from stechec to C.
*/
extern "C" void partie_init()
{
  PyObject* _retval = _call_python_function("partie_init");
  if (!_retval && PyErr_Occurred()) { PyErr_Print(); abort(); }
  try {
  Py_XDECREF(_retval);
  } catch (...) { PyErr_Print(); abort(); }
}

extern "C" void jouer_tour()
{
  PyObject* _retval = _call_python_function("jouer_tour");
  if (!_retval && PyErr_Occurred()) { PyErr_Print(); abort(); }
  try {
  Py_XDECREF(_retval);
  } catch (...) { PyErr_Print(); abort(); }
}

extern "C" void partie_fin()
{
  PyObject* _retval = _call_python_function("partie_fin");
  if (!_retval && PyErr_Occurred()) { PyErr_Print(); abort(); }
  try {
  Py_XDECREF(_retval);
  } catch (...) { PyErr_Print(); abort(); }
}

