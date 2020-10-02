// Feynder.cpp : Define the 'main' function.

#include <map>
#include <iterator>
#include <typeinfo>
#include <iostream>
#include <algorithm>
#include <string>

#include "Particles.h"
#include "Interactions.h"
#include "Diagram.h"

int main()
{
    const n0dict interactions{
        {P::phi,P::psi,P::antipsi},
    };

    //n1dict nto1{ generateDictionary(interactions) };

    Particle p1{ P::phi,true };
    Particle p2{ P::phi,true };
    Particle p3{ P::phi,true };
    Particle p4{ P::phi,true };
	Particle p5{ P::phi,true };

	std::pair<std::vector<std::vector<Particle>>, std::vector<Particle>> pairup{};
	pairup.second = { p1,p2,p3,p4 };
	auto list = getGroupings(pairup);

	for (auto thisgrouping : list) {
		for (auto group : thisgrouping) {
			for (auto number : group) {
				std::cout << number.getID();
			}
			std::cout << '|';
		}
		std::cout << '\n';
	}
}