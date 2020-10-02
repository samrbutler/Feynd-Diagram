// Particles.h : Set up classes and types relating to particles

#pragma once

#include <map>
#include <vector>
#include <set>
#include <tuple>

//Define particle names
enum class ParticleType {
	phi,
	psi,
	antipsi,
};

//Define alias
using P = ParticleType;

//Define the particle/antiparticle dictionary
inline const std::map<P, P> ParticleDict{
	{P::phi		,P::phi},
	{P::psi		,P::antipsi},
	{P::antipsi	,P::psi}
};

//Define a basic point class
class Point {

	//Set up a consecutive ID counter
	static int next_id;
	int m_id;

public:

	//Constructor
	Point() : m_id{ ++next_id }
	{
		//Initialiser list
	}

	//Private member mccess
	int getID() const { return m_id; }

	//Comparison functions
	bool operator<(Point& point2) const { return (m_id < point2.getID()); }
	bool operator==(Point& point2) const { return (m_id == point2.getID()); }
	bool operator!=(Point& point2) const { return (m_id != point2.getID()); }
};

//Extend the point class to include particles
class Particle : public Point {

	//Particle type
	P m_ptype;
	//Is this particle active in the algorithm
	bool m_active;
	//Is this an external particle
	bool m_isexternal;

public:

	//Private member access
	P getType() { return m_ptype; }
	bool isActive() { return m_active; }
	void setActive(bool act) { m_active = act; }
	bool isExternal() { return m_isexternal; }

	//Toggle the active status of the external point
	void toggleActive() { m_active = !m_active; }
	
	//No default constructor: you *must* define at least the particle type and externality on generation
	Particle() = delete;

	//Construct given particle type, externality and (optionally) activity
	Particle(P PType, bool isExtern, bool isAct = true) : m_ptype{ PType }, m_active{ isAct }, m_isexternal{ isExtern }
	{
		//Initialiser list
	}
};
