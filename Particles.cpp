/*Particles.cpp :
	Static class member variable initialisation
		- Initialise the unique point ID counter
	Operator overloads
		- std::ostream& operator<< (std::ostream& out, const P part)
			> Allow for output of ParticleTypes to std::cout
		- bool operator<(const std::vector<Particle>& vec1, const std::vector<Particle>& vec2)
			> Compare particle vectors by their first elements
		- bool operator!=(const std::vector<Particle>& part1, const std::vector<Particle>& part2)
			> Compare particle vectors for inequality
	Function definitions
		- P getAntiParticle(const P part)
			> Given a particle, return the name of its antiparticle
*/

#include "Particles.h"

#include "Model.h"

#include <iostream>
#include <vector>

//Initialise the unique point ID counter
int Point::next_id{ 0 };

//Allow for output of ParticleTypes to std::cout
std::ostream& operator<< (std::ostream& out, const P part) {
	//Switch based on the particle type
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

//Overload operator < to sort particle vectors by their first elements
bool operator<(const std::vector<Particle>& vec1, const std::vector<Particle>& vec2) { return (vec1[0] < vec2[0]); }

//Overload inequality operator for particle vectors
bool operator!=(const std::vector<Particle>& part1, const std::vector<Particle>& part2) {
	//Return unequal if there are different numbers of particles
	if (part1.size() != part2.size()) return true;

	//Return unequal if any of the particles are different
	for (size_t i{}; i < part1.size(); ++i) {
		if (part1[i] != part2[i]) return true;
	}

	//They must now be equal
	return false;
}
