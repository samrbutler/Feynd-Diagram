#include "Vertex.h"

Vertex::Vertex(const std::vector<Particle>& parts, const bool isProp)
{
	m_numlegs = static_cast<int>(parts.size());
	m_vertextype = {};
	m_isProp = isProp;
	for (const Particle& part : parts) {
		m_connection_ids.push_back(part.getID());
		m_connection_types.push_back(part.getType());
	}
}

//Add a point to the vertex if we can and return true if this was successful
bool Vertex::addLegs(const std::vector<int>& idstoadd, const std::vector<P>& typestoadd)
{
	//Return early if the number of legs will exceed capacity
	if (m_numlegs - m_connection_ids.size() - idstoadd.size() < 0) return false;
	else {
		//Go through and add all ids and types to the vertex
		for (size_t i{}; i < idstoadd.size(); ++i) {
			m_connection_ids.push_back(idstoadd[i]);
			m_connection_types.push_back(typestoadd[i]);
		}

		//Success
		return true;
	}
}

//Clean up the vertex for display - this is purely for ease of reading and does not need to be called for program operation
Vertex& Vertex::cleanup()
{
	//Pair up the ids and types ready for sorting
	std::vector<std::pair<int, P>> container(m_numlegs);
	for (int i{}; i < m_numlegs; ++i) {
		container[i] = std::make_pair(m_connection_ids[i], m_connection_types[i]);
	}
	//Sort the leg container based on the connection ids
	std::sort(container.begin(), container.end(), [](const std::pair<int, P>& x, const std::pair<int, P>& y) -> bool {return (x.first < y.first); });
	//Copy the sorted legs back into the vertex
	for (int i{}; i < m_numlegs; ++i) {
		m_connection_ids[i] = container[i].first;
		m_connection_types[i] = container[i].second;
	}
	return *this;
}

//Print out a vertex
std::ostream& operator<<(std::ostream& out, const Vertex& v)
{
	std::vector<int> legids{ v.getConnectionIDs() };
	std::vector<P> legtypes{ v.getConnectionTypes() };

	//If there are two particles at the vertex and it's a particle/antiparticle pair, this is a propagator...
	//...so display the propagator type and the connected point IDs
	if (legids.size() == 2) {
		out << "Propagator | ";
		out << legtypes[0] << " (" << legids[0] << ',' << legids[1] << ')';
	}
	//Otherwise this is a vertex, so display the connected point types and their IDs
	else {
		out << "    Vertex | ";
		for (size_t i{}; i < legids.size(); ++i) {
			out << legtypes[i] << " (" << legids[i] << ')';
			if (i != legids.size() - 1) out << ", ";
		}
	}
	return out;
}

//Print out a vector of vertices
std::ostream& operator<<(std::ostream& out, const std::vector<Vertex>& v)
{
	for (int i{}; i < v.size(); ++i) {
		out << v[i];
		if (i != v.size() - 1) out << '\n';
	}
	return out;
}