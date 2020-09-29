#include <vector>
#include "Vertices.h"
#include "Particles.h"
#include "Interactions.h"

//Initialise point ids
int Point::next_id{ 0 };

//Define default point constructor
Point::Point() {
	m_id = ++next_id;
}

//Construct a new vertex given a vector of _inbound_ particles
Vertex::Vertex(std::vector<ParticleType> &legs) {
	m_numlegs = legs.size();
	m_connected = false;
	m_available = legs;
	m_connections.reserve(m_numlegs);
}

External::External(P &type) {
	m_ptype = type;
}

Diagram::Diagram(std::vector<External> &externs) {
	m_externs = externs;
	
}