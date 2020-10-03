// Feynder.cpp : Define the 'main' function.
#include "Feynder.h"

#include "Diagram.h"
#include "Groups.h"
#include "Interactions.h"
#include "Particles.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <set>

std::ostream& operator<< (std::ostream& out, const P part) {
    switch (part) {
    case P::phi:
        out << "Phi";
        break;
    case P::psi:
        out << "Psi";
        break;
    case P::chi:
        out << "Chi";
        break;
    case P::antipsi:
        out << "Antipsi";
        break;
    }
    return out;
}



int main()
{
    const n0dict interactions{
        {P::phi,P::psi,P::antipsi},
        {P::chi,P::psi,P::antipsi}
    };

    n1dict dictionary{ generateDictionary(interactions) };

    
    Particle p1{ P::phi,true };
    Particle p2{ P::psi,true };
    Particle p3{ P::antipsi,true };
    Particle p4{ P::chi,true };

    std::vector<Particle> vec{ p1,p2,p3,p4 };
    grouping grp1;
    pairedgrouping input{ std::make_pair(grp1,vec) };
    auto returnval{ getGroupings(input) };
    for (auto element : returnval) {
        for (auto group : element.first) {
            for (auto part : group) {
                std::cout << part.getType() << ',';
            }
            std::cout << '|';
        }
        std::cout << "||";
        for (auto part : element.second) {
            std::cout << part.getType() << ',';
        }
        std::cout << '\n';
    }
    std::cout << "==================================="<<'\n';
    auto output{ getNewExterns(returnval,dictionary) };
    for (auto combo : output) {
        for (auto group : combo.first) {
            std::cout << group << '|';
        }
        std::cout << "||";
        for (auto part : combo.second) {
            std::cout << part.getType() << ',';
        }
        std::cout << '\n';
    }
}