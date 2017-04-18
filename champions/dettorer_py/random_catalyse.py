# coding: utf-8
# This file has been generated, if you wish to
# modify it in a permanent way, please refer
# to the script file : gen/generator_python.rb

# Abstract of this champion:
# - place the sample at the first valid position
#     - if no such position exists, transmute the largest area possible until it
#       does
# - give a random sample
# - when catalysts are available, catalyse a random valid position
#     - target player for catalyse is alternatively the opponent and the current
#       player, starting with the opponent


from api import *
import random


def transmute():
    biggest = None
    biggest_s = 0

    for x in range(TAILLE_ETABLI):
        for y in range(TAILLE_ETABLI):
            size = taille_region((x, y), moi())
            if size > biggest_s:
                biggest_s = size
                biggest = (x, y)

    transmuter(biggest)
    print("transmuter(({}, {}))".format(*biggest))


def place_sample():
    sample = echantillon_tour()
    valid_places = placements_possible_echantillon(sample, moi())
    while not valid_places:
        transmute()
        valid_places = placements_possible_echantillon(sample, moi())
    placer_echantillon(valid_places[0][0], valid_places[0][1])
    print("placer_echantillon({}, {})".format(valid_places[0][0], valid_places[0][1]))


def give_sampe():
    valid_elements = list(case_type)[1:]
    a, b = random.choice(valid_elements), random.choice(valid_elements)
    donner_echantillon((a, b))
    print("donner_echantillon(({}, {}))".format(str(a), str(b)))


last_catalysed = moi()
def use_catalyst():
    global last_catalysed
    target_player = moi() if last_catalysed == adversaire() else adversaire()
    target_pos = (random.randint(0, TAILLE_ETABLI), random.randint(0, TAILLE_ETABLI))
    while type_case(target_pos, target_player) == case_type.VIDE:
        target_pos = (random.randint(0, TAILLE_ETABLI), random.randint(0, TAILLE_ETABLI))

    orig_type = type_case(target_pos, target_player)
    valid_elements = list(case_type)
    del valid_elements[orig_type]
    del valid_elements[0]  # VIDE
    dest_type = random.choice(valid_elements)

    catalyser(target_pos, target_player, dest_type)
    print("catalyser({}, {}, {})".format(target_pos, target_player, str(dest_type)))

    last_catalysed = target_player


# Fonction appelée à chaque tour.
def jouer_tour():
    place_sample()
    give_sampe()
    while nombre_catalyseurs() > 0:
        use_catalyst()
    afficher_etablis()
    pass


# Fonction appelée au début de la partie.
def partie_init():
    pass


# Fonction appelée à la fin de la partie.
def partie_fin():
    # Place ton code ici
    pass
