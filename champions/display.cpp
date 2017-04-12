#include <algorithm>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <string>

#include "prologin.hh"

namespace {
	const std::string reset = "\033[0m";
	const std::string bold = "\033[1m";
	std::string fgcolor(unsigned c){ return "\033[3" + std::to_string(c) + "m"; }
	std::string bgcolor(unsigned c){ return "\033[4" + std::to_string(c) + "m"; }
}

void display(){
	std::cout << bgcolor(7) << fgcolor(0) << "moi";
	std::fill_n(std::ostream_iterator<char>(std::cout), TAILLE_ETABLI-5, ' ');
	std::cout << std::setw(4) << score(moi()) << reset << bold << "Tour:" << reset << bgcolor(7) << fgcolor(0) << "adv";
	std::fill_n(std::ostream_iterator<char>(std::cout), TAILLE_ETABLI-5, ' ');
	std::cout << std::setw(4) << score(adversaire()) << "\n";
	for(uint8_t y=0; y<TAILLE_ETABLI; ++y){
		std::cout << " ";
		for(uint8_t x=0; x<TAILLE_ETABLI; ++x)
			std::cout << bgcolor(type_case({y, x}, moi())) << " ";
		std::cout << bgcolor(7) << " " << reset;
		if(y==0) std::cout << std::setw(4) << tour_actuel() << " ";
		else if(y==1) std::cout << bold << "Reçu:";
		else if(y==2) std::cout << "  " << bgcolor(echantillon_tour().element1) << " " << bgcolor(echantillon_tour().element2) << " " << reset << " ";
		else std::cout << "     ";
		std::cout << bgcolor(7) << " ";
		for(uint8_t x=0; x<TAILLE_ETABLI; ++x)
			std::cout << bgcolor(type_case({y, x}, adversaire())) << " ";
		std::cout << bgcolor(7) << " \n";
	}
	std::fill_n(std::ostream_iterator<char>(std::cout), TAILLE_ETABLI+2, ' ');
	std::cout << reset << "     " << bgcolor(7);
	std::fill_n(std::ostream_iterator<char>(std::cout), TAILLE_ETABLI+2, ' ');
	std::cout << reset << "\n" << std::endl;
}
