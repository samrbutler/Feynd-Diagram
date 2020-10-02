// Diagram.cpp : Code relevant to diagram construction and manipulation
#include "Diagram.h"

#include "Groups.h"
#include "Interactions.h"
#include "Particles.h"

#include <vector>

bool Diagram::isVertex(n0dict& dictionary)
{
	if (static_cast<int>(m_externs.size()) == 1)
		return false;
	else if (static_cast<int>(m_externs.size()) == 2) {
		if (m_externs[0].getType() == getAntiParticle(m_externs[1].getType()))
			return true;
		else
			return false;
	}
	else {
		//Count the number of active Particles
		int activecount{ std::count_if(m_externs.begin(),m_externs.end(),[](Particle& p) -> bool {return p.isActive(); }) };
		//Return early if this is less than 2
		if (activecount < 2)
			return false;
		auto groupnames{ vec2multiset(m_externs) };
		//Loop over all interactions in the dictionary
		for (auto interaction : dictionary) {
			if (interaction == groupnames)
				return true;
		}
		return false;
	}
}
