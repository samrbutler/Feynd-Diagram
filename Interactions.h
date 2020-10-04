/*Interactions.h :
	Type aliases
		- n0dict
			> A dictionary for n->0 interactions
		- n1dict
			> A dictionary for n->1 interactions
		- listofproducts
			> A list of the products following the interactions prescribed by a grouping
	Classes
		- Vertex (inherits Point)
			> Extends the point class to add functionality for vertices
	Function declarations
		- vec2multiset
		- generateDictionary
		- isGroupingValid
		- getProducts
		- getNewExterns
*/

#pragma once

#include "Groups.h"
#include "Particles.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <set>

//A dictionary for n->0 interactions
using n0dict = std::multiset<std::multiset<P>>;
//A dictionary for n->1 interactions
using n1dict = std::set<std::pair<std::multiset<P>, P>>;
//A list of the products following the interactions prescribed by a grouping
using listofproducts = std::vector<std::vector<P>>;

//Extends the point class to add functionality for vertices
class Vertex : public Point {

	//Number of legs
	int m_numlegs;
	//IDs of points connected to the vertex
	std::vector<int> m_connection_ids;
	//Particle type of points connected to the vertex
	std::vector<P> m_connection_types;

public:

	//Return the list of point IDs that are connected to this vertex
	const std::vector<int>& getConnectionIDs() const { return m_connection_ids; }

	//Return the list of point types that are connected to this vertex
	const std::vector<P>& getConnectionTypes() const { return m_connection_types; }

	//You can't create an empty vertex
	Vertex() = delete;

	//Construct from a full list of legs
	Vertex(std::vector<int>& ids, std::vector<P> types) : m_numlegs{ static_cast<int>(ids.size()) }, m_connection_ids{ ids }, m_connection_types{ types } {}

	//Construct with a cutoff for the number of vertices
	Vertex(int legs) : m_numlegs{ legs }, m_connection_ids{}, m_connection_types{} {}

	bool addLegs(const std::vector<int>& idstoadd, const std::vector<P>& typestoadd);
};

std::multiset<P> vec2multiset(const std::vector<Particle>& group);
n1dict generateDictionary(const std::multiset<std::multiset<P>>&);
bool isGroupingValid(pairedgrouping& pair, const n1dict& dictionary);
std::vector<P> getProducts(std::vector<Particle>& group, const n1dict& dictionary);
listofproducts getNewExterns(pairedgrouping& pair, const n1dict& nto1);