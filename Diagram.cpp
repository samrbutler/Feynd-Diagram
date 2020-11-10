#include "Diagram.h"

//Construct a diagram from a vector of particles and a single vertex
Diagram::Diagram(const std::vector<Particle>& externs, const Vertex& vertex)
{
	m_vertices.push_back(vertex);
	m_externs = externs;
}

//Add a vector of vertices to the diagram
void Diagram::addVertices(const std::vector<Vertex>& verts)
{
	m_vertices.insert(m_vertices.end(), verts.begin(), verts.end());
}

//Return true if the external particles of the current diagram can form a vertex with two active particles
bool Diagram::isVertex(const n0dict& dictionary) const
{
	//If we only have 1 particle this isn't a vertex
	if (m_externs.size() == 1) return false;
	else {
		//Count the number of active particles: return early if this is less than 2
		if (std::count_if(m_externs.begin(), m_externs.end(), [](const Particle& p) -> bool {return p.isActive(); }) < 2) return false;

		//Check if we have 2 particles
		if (m_externs.size() == 2) {
			//Check if this is a propagator (i.e. a particle and its antiparticle meet)
			if (m_externs[0].getType() == getAntiParticle(m_externs[1].getType())) return true;

			//If it isn't a propagator, this isn't a vertex (this will change in the future)
			else return false;
		}
		//We now have more than 2 particles
		else {
			//Get the types of the particles
			std::multiset<P> types{ vec2multiset(m_externs) };

			//Search for this interaction in the n->0 dictionary
			for (const std::multiset<P>& interaction : dictionary) {
				//If we find it, this is a vertex
				if (interaction == types) return true;
			}

			//If it wasn't found, this isn't a vertex
			return false;
		}
	}
}

//Clean up the diagram for display
Diagram& Diagram::cleanup()
{
	//Set up an ID map - we're going to reduce all the particle IDs
	std::map<int, int> id_map;
	int counter{ 1 };
	for (Particle& p : m_externs) {
		//Get the particle's ID at present and find it in the map
		int i{ p.getID() };
		auto loc{ id_map.find(i) };
		//If we found the ID in the map, perform the appropriate replacement 
		if (loc != id_map.end()) p.setID(loc->second);
		//Otherwise...
		else {
			//Add an entry in the map for future lookups
			id_map.insert(std::make_pair(i, counter));
			//Set the ID appropriately and then increment counter
			p.setID(counter++);
		}
	}
	//Do the same with the vertices
	for (Vertex& v : m_vertices) {
		for (int& i : v.getModifiableConnectionIDs()) {
			auto loc{ id_map.find(i) };
			if (loc != id_map.end()) i = loc->second;
			else {
				id_map.insert(std::make_pair(i, counter));
				i = counter++;
			}
		}
		//Clean up the individual vertices
		v.cleanup();
	}
	//Sort the vertices according to the connection ID of their first elements
	std::sort(m_vertices.begin(), m_vertices.end(), [](const Vertex& v1, const Vertex& v2) -> bool { return (v1.getConnectionIDs()[0]< v2.getConnectionIDs()[0]); });
	return *this;
}

//Allow for output of a diagram to std::cout
std::ostream& operator<< (std::ostream& out, const Diagram& diag)
{
	std::cout << diag.getExterns() << '\n';
	//Loop through the vertices
	for (const Vertex& vertex : diag.getVertices()) {
		std::cout << vertex << '\n';
	}
	return out;
}