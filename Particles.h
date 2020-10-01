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
	int m_id{};

public:

	//Default constructor with initialisation list
	Point() : m_id{ ++next_id }
	{
		//No explicit constructor
	}
};

//Extend the point class to include particles
class Particle : public Point {

	//Particle type
	const P m_ptype;
	//Is this particle active in the algorithm
	bool m_active;
	//Is this an external particle
	const bool m_isexternal;

public:

	//Private member access
	P getType() { return m_ptype; }
	bool isActive() { return m_active; }
	void setActive(bool act) { m_active = act; }
	bool isExternal() { return m_isexternal; }

	//Toggle the active status of the external point
	void toggleActive() { m_active = !m_active; }
	
	//No default constructor: you *must* define the particle type and externality on generation
	Particle() = delete;

	//Constructor with initialisation list
	Particle(P& PType, bool isExtern, bool isAct = true) : m_ptype{ PType }, m_active{ isAct }, m_isexternal{ isExtern }
	{
		//No explicit constructor
	}
};