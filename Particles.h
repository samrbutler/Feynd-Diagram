/*Groups.h :
	Classes
		- Point
			> A point container class
		- Particle (inherits Point)
			> Extends the point class to add functionality for particles
	Function declarations
		- operator<<
		- operator<
		- operator!=
		- getAntiParticle
*/

#pragma once

#include "Model.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <tuple>
#include <vector>

//A point container class
class Point {

	//Consecutive ID counter
	static int next_id;
	int m_id;

public:

	//Default constructor to initialise the ID
	Point() : m_id{ ++next_id } {}

	//Return the ID of the point
	int getID() const { return m_id; }

	//Overload operator< to compare point IDs
	bool operator<(const Point& point2) const { return (m_id < point2.getID()); }
	//Overload operator> to compare point IDs
	bool operator>(const Point& point2) const { return (m_id > point2.getID()); }
	//Overload operator== to compare point IDs
	bool operator==(const Point& point2) const { return (m_id == point2.getID()); }
	//Overload operator!= to compare point IDs
	bool operator!=(const Point& point2) const { return (m_id != point2.getID()); }
};

//Extends the point class to add functionality for particles
class Particle : public Point {

	//Particle type
	P m_ptype;
	//Is this particle marked as active for the algorithm
	bool m_active;

public:

	//Return the type of the particle
	P getType() const { return m_ptype; }
	//Return the activity status of the particle
	bool isActive() const { return m_active; }

	//Set the activity status of the particle
	void setActive(bool act) { m_active = act; }
		
	//Delete the default constructor: you *must* define at least the particle type on generation
	Particle() = delete;

	//Construct given particle type and (optionally) activity
	Particle(const P PType, const bool isAct = true) : m_ptype{ PType }, m_active{ isAct } {}
};

std::ostream& operator<<(std::ostream& out, const P part);
bool operator<(const std::vector<Particle>& vec1, const std::vector<Particle>& vec2);
bool operator!=(const std::vector<Particle>& part1, const std::vector<Particle>& part2);