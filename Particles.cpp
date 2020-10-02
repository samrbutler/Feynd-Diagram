// Particles.cpp : Introduce code for particles and their actions
#include "Particles.h"

#include "Interactions.h"

#include <vector>

//Initialise Point ids
int Point::next_id{ 0 };

//Print out particle names
void print(P part)
{
    if (part == P::phi)
        std::cout << "Phi";
    else if (part == P::psi)
        std::cout << "Psi";
    else if (part == P::antipsi)
        std::cout << "Antipsi";
    else if (part == P::chi)
        std::cout << "Chi";
}

//Switch to the antiparticle
P getAntiParticle(P part) {
	return AntiParticleDict.at(part);
}