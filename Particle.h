#pragma once

#include "Model.h"
#include "Point.h"

#include <iterator>
#include <iostream>
#include <vector>

//Extends the point class to add functionality for particles
class Particle : public Point
{

	//Particle type
	P m_ptype;
	//Is this particle marked as active for the algorithm
	bool m_active;
	//Which loops does this particle "belong" to
	std::vector<int> m_loops;
	//Is this particle external, i.e. is it allowed to carry loop momentum?
	bool m_external;
	//What particles is this identical to?
	int m_identicality;

public:

	//Return the type of the particle
	P getType() const { return m_ptype; }
	//Return the activity status of the particle
	bool isActive() const { return m_active; }
	//Return the loop participation
	std::vector<int> getLoops() const { return m_loops; }
	//Return the identicality index
	int getIdenticality() const { return m_identicality; }
	//Set the identicality index
	void setIdenticality(int ical) { m_identicality = ical; }
	bool hasIdenticality() const { return m_identicality != -1; }

	//Add and subtract loop momenta from the current particle (a negative input removes the loop)
	Particle addLoop(const std::vector<int> loop_ids);

	//Set the activity status of the particle
	Particle setActive(const bool act);

	//Toggle to the antiparticle
	Particle toggleAntiPart();

	//Delete the default constructor: you *must* define at least the particle type on generation
	Particle() = delete;

	//Construct given particle type and (optionally) activity and externality
	Particle(const P PType, const bool isAct = true, const bool isExt = true)
		: m_ptype{ PType }, m_active{ isAct }, m_loops{}, m_external{ isExt }, m_identicality{ -1 } {}

	//Construct given particle type, activity and the loop property
	Particle(const P PType, const bool isAct, const std::vector<int> loops)
		: m_ptype{ PType }, m_active{ isAct }, m_loops{ loops }, m_external{ false }, m_identicality{ -1 } {}
};

bool operator<(const std::vector<Particle>& vec1, const std::vector<Particle>& vec2);

bool operator!=(const std::vector<Particle>& part1, const std::vector<Particle>& part2);

std::ostream& operator<<(std::ostream& out, const Particle& p);

std::ostream& operator<<(std::ostream& out, const std::vector<Particle>& v);