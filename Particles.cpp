/*Particles.cpp : Code relevant to the Particle class and for manipulating instances
*	Static class member variable initialisation
*		- Initialise the unique point ID counter
*	Operator overloads
*		- comparison operators for Point
*		- operator<< (P)
*			> Allow for output of ParticleTypes to std::cout
*		- operator< (std::vector<Particle>)
*			> Compare particle vectors by their first elements
*		- operator!= (std::vector<Particle>)
*			> Compare particle vectors for inequality
*	Function definitions
*		- getAntiParticle
*			> Given a particle, return the name of its antiparticle
*/

#include "Particles.h"

#include "Model.h"

#include <iostream>
#include <vector>

//Initialise the unique point ID counter
int Point::next_id{ 0 };

//Overload operator< to compare point IDs
bool operator<(const Point& point1, const Point& point2) { return (point1.getID() < point2.getID()); }
//Overload operator> to compare point IDs
bool operator>(const Point& point1, const Point& point2) { return (point1.getID() > point2.getID()); }
//Overload operator== to compare point IDs
bool operator==(const Point& point1, const Point& point2) { return (point1.getID() == point2.getID()); }
//Overload operator!= to compare point IDs
bool operator!=(const Point& point1, const Point& point2) { return (point1.getID() != point2.getID()); }


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
