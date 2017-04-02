/// This file has been generated, if you wish to
/// modify it in a permanent way, please refer
/// to the script file : gen/generator_cxx.rb

#include <bits/stdc++.h>
#include "prologin.hh"

const bool PRINT = true;
const std::vector<case_type> itocase = {VIDE, PLOMB, FER, CUIVRE, SOUFRE, MERCURE};

std::vector<std::vector<case_type>> get_map(int id) {
    std::vector<std::vector<case_type>> map(6, std::vector<case_type>(TAILLE_ETABLI));
    for (int i = 0; i < TAILLE_ETABLI; i++) for (int j = 0; j < TAILLE_ETABLI; j++) {
        map[i][j] = type_case({i, j}, id);
    }
    return map;
}

void print_map(auto map) {
    for (int i = 0; i < TAILLE_ETABLI; i++) {
        for (int j = 0; j < TAILLE_ETABLI; j++) {
            switch (map[i][j]) {
                case VIDE:
                    if (PRINT) std::cout << ".";
                    break;
                case PLOMB:
                    if (PRINT) std::cout << "P";
                    break;
                case FER:
                    if (PRINT) std::cout << "F";
                    break;
                case CUIVRE:
                    if (PRINT) std::cout << "C";
                    break;
                case SOUFRE:
                    if (PRINT) std::cout << "S";
                    break;
                case MERCURE:
                    if (PRINT) std::cout << "M";
                    break;
            }
        }
        if (PRINT) std::cout << std::endl;
    }
    if (PRINT) std::cout << std::endl;
}

/// Fonction appelée au début de la partie.
void partie_init()
{
    if (PRINT) std::cout << "BEGIN MATCH" << std::endl;
    return;
}

/// Fonction appelée à chaque tour.
void jouer_tour()
{
    // Get the map
    auto map = get_map(moi());
    print_map(map);

    // Place the sample at the first possible location, and transmute regions if there isn't one.
    auto sample_given = echantillon_tour();
    auto positions = placements_possible_echantillon(sample_given, moi());
    while (positions.size() == 0u) {
        bool done = false;
        for (int i = 0; i < TAILLE_ETABLI; i++) for (int j = 0; j < TAILLE_ETABLI; j++) {
            if (done) break;
            if (propriete_case({i, j}, moi()) != AUCUNE) {
                transmuter({i, j});
                done = true;
            }
        }
        positions = placements_possible_echantillon(sample_given, moi());
    }
    auto choosen_pos = positions[0];
    placer_echantillon(choosen_pos.pos1, choosen_pos.pos2);

    // Set the next sample as the two elements mostly present in the map, with at least one from the last sample received.
    std::vector<int> presence(NB_TYPE_CASES);
    for (int i = 0; i < TAILLE_ETABLI; i++) for (int j = 0; j < TAILLE_ETABLI; j++) {
        presence[map[i][j]]++;
    }    
    presence[VIDE] = -1;
    int p1 = 0, p2 = 0;
    echantillon sample_to_give;
    for (int i = 0; i < NB_TYPE_CASES; i++) {
        if (presence[i] > p1) {p1 = presence[i]; sample_to_give.element1 = itocase[i];}
        if (presence[i] > p2 && (i == sample_given.element1 || i == sample_given.element2)) {p2 = presence[i]; sample_to_give.element2 = itocase[i];}
    }
    donner_echantillon(sample_to_give);
}

/// Fonction appelée à la fin de la partie.
void partie_fin()
{
    return;
}

