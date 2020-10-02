// Interactions.h : Set up classes and types relating to interactions of particles

#pragma once

#include <set>
#include <map>
#include <iterator>
#include <iostream>

#include "Particles.h"

//Define a type alias for the n->1 interaction dictionary
using n1dict = std::set<std::multimap<std::multiset<P>, P>>;

//Define a type alias for the n->0 interaction dictionary
using n0dict = std::multiset<std::multiset<P>>;

//Function prototype for generation of the n->1 particle dictionary
n1dict generateDictionary(const std::multiset<std::multiset<P>>&);

//Define a vertex class
class Vertex : public Point {

	//Number of legs
	int m_numlegs;
	//Connections
	std::vector<Point> m_connections;

public:

	//Default constructor
	Vertex() = default;

	//Construct from a full list of vertices
	Vertex(std::vector<Point>& vec) : m_numlegs{ static_cast<int>(vec.size()) }, m_connections{ vec }
	{
		//Initialiser list
	}

	//Construct with a cutoff for the number of vertices
	Vertex(int legs) : m_numlegs{ legs }, m_connections{}
	{
		//Initialiser list
	}

	//Add a leg: returns true if this was successful
	bool addLeg(Point);
};