/*Interactions.h :
*
*/

#pragma once

#include "Groups.h"
#include "Model.h"
#include "Particles.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <vector>

//A list of the products following the interactions prescribed by a grouping
using listofproducts = std::vector<std::vector<P>>;

//Extends the point class to add functionality for vertices
class Vertex : public Point {

	//Number of legs
	int m_numlegs;
	//Vertex type (for future use)
	int m_vertextype;
	//Define if this vertex is a propagator (for future use)
	bool m_isProp;
	//IDs of points connected to the vertex
	std::vector<int> m_connection_ids;
	//Particle type of points connected to the vertex
	std::vector<P> m_connection_types;

public:

	//Return the list of point IDs that are connected to this vertex
	const std::vector<int>& getConnectionIDs() const { return m_connection_ids; }
	//Return a modifiable list of point IDs that are connected to this vertex
	std::vector<int>& getModifiableConnectionIDs() { return m_connection_ids; }

	//Return the list of point types that are connected to this vertex
	const std::vector<P>& getConnectionTypes() const { return m_connection_types; }

	//Return the vertex type
	const int getVertexType() const { return m_vertextype; }

	//You can't create an empty vertex
	Vertex() = delete;

	//Construct given a list of particles
	Vertex(const std::vector<Particle>& parts, const bool isProp = false);

	//Construct with a cutoff for the number of vertices
	Vertex(int legs) : m_numlegs{ legs }, m_vertextype{}, m_isProp{}, m_connection_ids{}, m_connection_types{} {}

	Vertex& cleanup();

	bool addLegs(const std::vector<int>& idstoadd, const std::vector<P>& typestoadd);
};

std::ostream& operator<<(std::ostream& out, const Vertex& v);
std::ostream& operator<<(std::ostream& out, const std::vector<Vertex>& v);

std::multiset<P> vec2multiset(const std::vector<Particle>& group);
bool isGroupingValid(const pairedgrouping& pair, const n1dict& dictionary = Model::n_to_1);
std::vector<P> getProducts(const std::vector<Particle>& group, const n1dict& dictionary = Model::n_to_1);
listofproducts getNewExterns(const pairedgrouping& pair, const n1dict& nto1 = Model::n_to_1);