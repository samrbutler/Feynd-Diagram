// Interactions.h : Set up classes and types relating to interactions of particles

#pragma once

#include "Groups.h"
#include "Particles.h"

#include <iostream>
#include <iterator>
#include <map>
#include <set>

using n0dict = std::multiset<std::multiset<P>>;
using n1dict = std::set<std::pair<std::multiset<P>, P>>;

using listofpairedproducts = std::vector<std::pair<std::vector<P>, std::vector<Particle>>>;

n1dict generateDictionary(const std::multiset<std::multiset<P>>&);

//Extend the point class to include vertices
class Vertex : public Point {

	//Number of legs
	int m_numlegs;
	//Connections
	std::vector<int> m_connection_ids;

public:

	//Default constructor
	Vertex() = default;

	//Construct from a full list of legs
	Vertex(std::vector<int>& vec) : m_numlegs{ static_cast<int>(vec.size()) }, m_connection_ids{ vec }
	{
		//Initialiser list
	}

	//Construct with a cutoff for the number of vertices
	Vertex(int legs) : m_numlegs{ legs }, m_connection_ids{}
	{
		//Initialiser list
	}

	//Add a leg: returns true if this was successful
	bool addLegs(std::vector<int>& pointstoadd);
};

std::multiset<P> vec2multiset(std::vector<Particle>& group);

bool isGroupingValid(pairedgrouping& pair, const n1dict& dictionary);

std::vector<P> getProducts(std::vector<Particle>& group, const n1dict& dictionary);

listofpairedproducts getNewExterns(listofpairedgroupings& grps, const n1dict& nto1);