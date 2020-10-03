// Diagram.cpp : Code relevant to diagram construction and manipulation
#include "Diagram.h"

#include "Groups.h"
#include "Interactions.h"
#include "Particles.h"
#include <iterator>
#include <vector>

Diagram::Diagram(std::vector<Particle>& externs, Vertex vertex) {
	m_vertices.push_back(vertex);
	m_externs = externs;
}

bool Diagram::isVertex(const n0dict& dictionary)
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

//Create a process from incoming and outgoing particle names
Process::Process(std::vector<P>& incoming, std::vector<P>& outgoing) {
	for (P part : incoming) {
		//Construct an active particle and add it to the list
		m_externs.push_back(Particle(part, true));
	}
	for (P part : outgoing) {
		//Construct an active antiparticle and add it to the list
		m_externs.push_back(Particle(getAntiParticle(part), true));
	}
	//Store the particle names
	m_incoming = incoming ;
	m_outgoing = outgoing ;
}

std::vector<Diagram> connect(Diagram& proc, const n0dict& nto0, const n1dict& nto1) {
	auto externs{ proc.getExterns() };
	int s{ static_cast<int>(externs.size()) };
	//Check to see if we can form a vertex
	if (proc.isVertex(nto0)) {
		//If we can, add all particle IDs to a list
		std::vector<int> vec{};
		for (Particle part : externs) {
			vec.push_back(part.getID());
		}
		//Return a diagram consisting of the external points and a vertex connecting them
		return std::vector<Diagram> {Diagram(externs, Vertex(vec))};
	}
	//If there are two or fewer particles left, this process has failed, so return an empty vector
	else if (s<=2) {
		return {};
	}
	else {
		//Construct the initial (un)grouped vertex list
		pairedgrouping input{ std::make_pair(grouping{},externs) };
		//Get all possible groupings
		listofpairedgroupings groupinglist{ getGroupings(input) };
		//Get a list of possible new vertices
		listofpairedproducts vertexlist{ getNewExterns(groupinglist, nto1) };
		//TODO: Go through the 
	}
}