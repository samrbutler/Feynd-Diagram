/*Diagram.cpp :
*	TO DO:
*		- When we have the ability to do so, modify isVertex to search for loop topologies as well
*/

#include "Diagram.h"

#include "Groups.h"
#include "Interactions.h"
#include "Model.h"
#include "Particles.h"

#include <iterator>
#include <vector>

//Construct a diagram from a vector of particles and a single vertex
Diagram::Diagram(const std::vector<Particle>& externs, const Vertex& vertex) {
	m_vertices.push_back(vertex);
	m_externs = externs;
}

//Add a vector of vertices to the diagram
void Diagram::addVertices(const std::vector<Vertex>& verts) {
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

//Create a process from incoming and outgoing particle names
Process::Process(const std::vector<P> incoming, const std::vector<P> outgoing) {
	for (P part : incoming) {
		//Construct an active particle and add it to the list
		m_externs.push_back(Particle(part, true));
	}
	for (P part : outgoing) {
		//Construct an active antiparticle and add it to the list
		m_externs.push_back(Particle(getAntiParticle(part), true));
	}
	//Store the particle names
	m_incoming = incoming;
	m_outgoing = outgoing;
}

//Allow for output of a diagram to std::cout
std::ostream& operator<< (std::ostream& out, const Diagram& diag) {
	std::cout << "\t Externals | ";
	for (const Particle& part : diag.getExterns()) {
		std::cout << part.getType() << " (" << part.getID() << "), ";
	}
	std::cout << '\n';
	//Loop through the vertices
	for (const Vertex& vertex : diag.getVertices()) {
		//Get particle information
		std::vector<int> legids{ vertex.getConnectionIDs() };
		std::vector<P> legtypes{ vertex.getConnectionTypes() };

		//If there are two particles at the vertex and it's a particle/antiparticle pair, this is a propagator...
		//...so display the propagator type and the connected point IDs
		if (legids.size() == 2) {
			std::cout << "\tPropagator | ";
			std::cout << legtypes[0] << " (" << legids[0] << "," << legids[1] << "), ";
		}

		//Otherwise this is a vertex, so display the connected point types and their IDs
		else {
			std::cout << "\t    Vertex | ";
			for (size_t i{}; i < legids.size(); ++i) {
				std::cout << legtypes[i] << " (" << legids[i] << "), ";
			}
		}
		std::cout << '\n';
	}

	return out;
}

//Given a diagram and interaction dictionaries, return a vector of all possible completed tree-level diagrams
std::vector<Diagram> connect(Diagram& diag, const n0dict& nto0, const n1dict& nto1, bool debug) {

	/* ALGORITHM
	*	> Check if the external particles can connect directly into a vertex with at least two active particles
	*		> If they can, add the vertex and return the completed diagram
	*	> Check if we have two or fewer particles left without being able to connect them
	*		> If we do, the diagram has not connected, so return an empty vector
	*	> Otherwise, get all possible groupings of the external particles
	*		> For each grouping, get all possible products
	*			> If there are no products, skip this grouping
	*			> If each group does not include at least one active particle, skip this grouping
	*			> Go through all possible products and get the new external particles.
					> Set these new particles active, the other particles passive and make a subdiagram
	*				> Use recursion to connect the subdiagram
	*					> If the subdiagram returned connections, combine the subdiagram into the current diagram and add it to the list
	*					> If the subdiagram fails to connect, go to the next product
	*			> If all products failed to produce a connected diagram, go to the next grouping
	*		> If all groupings failed to connect, the diagram has failed to connect, so return an empty vector
	*/

	//Store the current external vertices of this diagram
	const std::vector<Particle>& externs{ diag.getExterns() };

	//DEBUG: Print details about all the current external particles
	if (debug) {
		std::cout << "Current External Particles\n";
		for (const Particle& part : externs) {
			std::cout << part.getType() << " , " << part.isActive() << " | ";
		}
		std::cout << '\n';
		std::cout << "---------------------------------------------\n";
	}

	//Store the number of external particles
	size_t s{ externs.size() };

	//Check to see if we can form a vertex
	if (diag.isVertex(nto0)) {
		//DEBUG: Declare this to be a vertex
		if (debug) {
			std::cout << "IS VERTEX : DONE\n";
			std::cout << "=============================================\n";
		}

		//Return a single diagram consisting of the external points and a vertex connecting them
		return std::vector<Diagram> {Diagram(externs, Vertex(externs))};
	}

	//If we don't have a vertex but there are now two or fewer particles left, this process has failed, so return an empty vector
	else if (s <= 2) {
		//DEBUG: Declare a failure
		if (debug) {
			std::cout << "FAILED : RETURNING\n";
			std::cout << "=============================================\n";
		}

		//Return an empty vector
		return {};
	}
	else {
		//Create empty return vector to add Diagrams to
		std::vector<Diagram> returnvec;

		//Construct the initial (un)grouped vertex list
		pairedgrouping input{ std::make_pair(grouping{},externs) };

		//Get all possible groupings
		listofpairedgroupings groupinglist{ getGroupings(input) };

		//DEBUG : Display all groupings
		if (debug) {
			std::cout << "FULL GROUPING LIST\n";
			for (const pairedgrouping& pg : groupinglist) {
				for (const std::vector<Particle>& group : pg.first) {
					for (const Particle& part : group) {
						std::cout << part.getType() << ',';
					}
					std::cout << '|';
				}
				for (const Particle& part : pg.second) {
					std::cout << part.getType() << ',';
				}
				std::cout << '\n';
			}
			std::cout << "---------------------------------------------\n";
		}

		//For each grouping in the list...
		for (pairedgrouping grp : groupinglist) {
			//...get a list of possible new products for this group
			listofproducts prodlist{ getNewExterns(grp, nto1) };
			//DEBUG : Display the current grouping and what it will reduce to
			if (debug) {
				std::cout << "Current Grouping: \n\t";
				for (const std::vector<Particle>& group : grp.first) {
					for (const Particle& part : group) {
						std::cout << part.getType() << ',';
					}
					std::cout << "| ";
				}
				std::cout << " |X| ";
				for (const Particle& part : grp.second) {
					std::cout << part.getType() << ',';
				}
				std::cout << '\n';
				std::cout << "Product lists: \n";
				for (const std::vector<P>& combo : prodlist) {
					std::cout << '\t';
					for (const P group : combo) {
						std::cout << group << "| ";
					}
					std::cout << '\n';
				}
			}

			//Skip this grouping if it doesn't produce any valid interactions
			if ((prodlist.size() == 0 || prodlist.size() == 1) && (prodlist[0].size() == 0)) {
				//DEBUG : Declare the grouping to be invalid
				if (debug) {
					std::cout << "GROUPING INVALID : CONTINUING\n";
					std::cout << "---------------------------------------------\n";
				}

				//Skip
				continue;
			}

			//Go through each possible interaction product
			for (size_t j{}; j < prodlist.size(); ++j) {
				//Set up the new particle and vertex containers
				std::vector<Particle> particlestoadd;
				std::vector<Vertex> verticestoadd;

				//Go through each of the groups that have been formed
				for (size_t i{}; i < grp.first.size(); ++i) {
					//Set up the new particle information containers for vertex production
					std::vector<Particle> vertexparts {grp.first[i]};

					//Add the new (anti)particle to the external container and add it to the vertex
					Particle p_to_add(prodlist[j][i], true);
					particlestoadd.push_back(p_to_add);
					vertexparts.push_back(p_to_add.toggleAntiPart());

					//DEBUG: Print out the vertex that's going to be added
					if (debug) {
						std::cout << "Adding a vertex:\n\t";
						for (size_t k{}; k < vertexparts.size(); ++k) {
							std::cout << vertexparts[k].getType() << ", " << vertexparts[k].getID() << " | ";
						}
						std::cout << '\n';
					}

					//Create and add the vertex to the current diagram
					verticestoadd.push_back(Vertex(vertexparts));
				}

				//Now add the previous external particles to the new container, setting them inactive first
				for (Particle& ungrouped : grp.second) {
					particlestoadd.push_back(ungrouped.setActive(false));
				}

				//DEBUG: Display the number of new particles
				if (debug) {
					std::cout << "Adding " << particlestoadd.size() << " new particles to a subdiagram.\nInitiating recursion.\n";
					std::cout << "---------------------------------------------\n";
				}

				//At this point we are ready to create a new subdiagram
				Diagram subdiag{ particlestoadd };

				//Initiate a recursion by connecting the subdiagram
				std::vector<Diagram> connectedsds{ connect(subdiag, nto0, nto1, debug) };

				//If we got nothing back, this grouping has failed
				if (connectedsds.size() == 0) {
					//DEBUG: Declare a failure to connect the subdiagram
					if (debug) {
						std::cout << "SUBDIAGRAM FAILED TO CONNECT\n";
						std::cout << "---------------------------------------------\n";
					}

					//Skip
					continue;
				}

				//If we got diagrams back, for each subdiagram create a new diagram which combines the vertices... 
				//...and external particles of the current diagram with the vertices of the subdiagram
				for (Diagram& sd : connectedsds) {
					Diagram newdiag(diag);
					newdiag.addVertices(verticestoadd);
					newdiag.addVertices(sd.getVertices());

					//And add this new diagram to the return vector
					returnvec.push_back(newdiag);
				}
			}

		}

		//DEBUG: Declare a return up the chain recursion
		if (debug) {
			std::cout << "RETURNING\n";
			std::cout << "=============================================\n";
		}

		//We're out of options so jump back up the chain
		return returnvec;
	}
}