
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

void print_map(const std::vector<std::vector<case_type>> &map) {
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
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist(0, 100);

    // Get the map
    std::vector<std::vector<case_type>> map = get_map(moi());
    print_map(map);

    // Place the sample at the first possible location, and transmute regions if there isn't one.
    echantillon sample_given = echantillon_tour();
    if (PRINT) std::cout << "tour : " << tour_actuel() << std::endl;
    if (PRINT) std::cout << sample_given.element1 << " " << sample_given.element2 << std::endl;
    std::vector<position_echantillon> positions = placements_possible_echantillon(sample_given, moi());
    while (positions.size() == 0u) {
        bool done = false;
        for (int i = 0; i < TAILLE_ETABLI; i++) for (int j = 0; j < TAILLE_ETABLI; j++) {
            if (done) break;
            if (propriete_case({i, j}, moi()) != AUCUNE) {
                erreur e_t = transmuter({i, j});
                if (e_t != OK) std::cout << "transmuter : " << e_t << std::endl;
                done = true;
            }
        }
        positions = placements_possible_echantillon(sample_given, moi());
    }
    position_echantillon best_pos;
    int best_size = -1;
    for (position_echantillon p : positions) {
        if (PRINT) std::cout << p.pos1.ligne << " " << p.pos1.colonne << " " << p.pos2.ligne << " " << p.pos2.colonne << std::endl;
        erreur e_pe = placer_echantillon(p.pos1, p.pos2);
        if (e_pe != OK) std::cout << "placer_test : " << e_pe << std::endl;
        int size_max = -1;
        for (int i = 0; i < TAILLE_ETABLI; i++) for (int j = 0; j < TAILLE_ETABLI; j++) {
            size_max = std::max(size_max, taille_region({i, j}, moi()));
        }
        if (size_max > best_size) {
            best_size = size_max;
            best_pos = p;
        }
        if (e_pe == OK) annuler();
    }
    erreur e_pe = placer_echantillon(best_pos.pos1, best_pos.pos2);
    if (e_pe != OK) std::cout << "placer : " << e_pe << std::endl;

    // If last turn, transmute everything
    if (tour_actuel() >= NB_TOURS - 1) for (int i = 0; i < TAILLE_ETABLI; i++) for (int j = 0; j < TAILLE_ETABLI; j++)
        transmuter({i, j});


    // Set the next sample as the two elements mostly present in the map, with at least one from the last sample received.
    std::vector<int> presence(NB_TYPE_CASES);
    for (int i = 0; i < TAILLE_ETABLI; i++) for (int j = 0; j < TAILLE_ETABLI; j++) {
        presence[map[i][j]]++;
    }    
    presence[VIDE] = -1;
    int p1 = -1, p2 = -1;
    echantillon sample_to_give;
    for (int i = 0; i < NB_TYPE_CASES; i++) {
        if (presence[i] > p1) {p1 = presence[i]; sample_to_give.element1 = itocase[i];}
        if (presence[i] > p2 && (i == sample_given.element1 || i == sample_given.element2)) {p2 = presence[i]; sample_to_give.element2 = itocase[i];}
    }
    int mean = uniform_dist(e1);
    if (mean > 30) sample_to_give.element1 = itocase[1 + rand() % 5];
    if (PRINT) std::cout << mean << std::endl;
    erreur e_de = donner_echantillon(sample_to_give);
    if (e_de != OK) std::cout << "donner : " << e_de << std::endl;
    if (PRINT) std::cout << sample_to_give.element1 << " " << sample_to_give.element2 << std::endl;
    if (PRINT) std::cout << std::endl;

    // Use of catalysts if there is some
    if (PRINT) std::cout << "Nombre de catalyseurs : " << nombre_catalyseurs() << std::endl;
    while (nombre_catalyseurs() > 0) {
        int max_size = -1;
        int i_max = 0, j_max = 0, m_max = 0;
        for (int i = 0; i < TAILLE_ETABLI; i++) for (int j = 0; j < TAILLE_ETABLI; j++) for (int m = 1; m < 6; m++) {
            erreur e_c = catalyser({i, j}, moi(), itocase[m]);
            int size = taille_region({i, j}, moi());
            if (size > max_size) {
                max_size = size;
                i_max = i;
                j_max = j;
                m_max = m;
            }
            if (e_c == OK) annuler();
        }
        if (catalyser({i_max, j_max}, moi(), itocase[m_max]) != OK) break;
    }
}

/// Fonction appelée à la fin de la partie.
void partie_fin()
{
    return;
}

