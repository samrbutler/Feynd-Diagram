/*Diagram.cpp :
	Vertex member function definitions
		- Diagram::Diagram(std::vector<Particle>& externs, Vertex vertex)
			> Construct a diagram from a vector of particles and a single vertex
		- void Diagram::addVertices(std::vector<Vertex> verts)
			> Add a vector of vertices to the diagram
		- bool Diagram::isVertex(const n0dict& dictionary)
			> Return true if the external particles of the current diagram can form a vertex with two active particles
	Process member function definitions	
		- Process::Process(std::vector<P> incoming, std::vector<P> outgoing)
			> Create a process from incoming and outgoing particle names
	Operator overloads
		- std::ostream& operator<< (std::ostream& out, const Diagram& diag)
			> Allow for output of a diagram to std::cout
	Function definitions
		- std::vector<Diagram> connect(Diagram& diag, const n0dict& nto0, const n1dict& nto1, bool debug = false)
			> Given a diagram and interaction dictionaries, return a vector of all possible completed tree-level diagrams
			> Set debug to true for verbose terminal output
*/

#include "Diagram.h"

#include "Groups.h"
#include "Interactions.h"
#include "Particles.h"

#include <iterator>
#include <vector>

//Construct a diagram from a vector of particles and a single vertex
Diagram::Diagram(std::vector<Particle>& externs, Vertex vertex) {
	m_vertices.push_back(vertex);
	m_externs = externs;
}

//Add a vector of vertices to the diagram
void Diagram::addVertices(std::vector<Vertex> verts) {
	m_vertices.insert(m_vertices.end(), verts.begin(), verts.end());
}

//Return true if the external particles of the current diagram can form a vertex with two active particles
bool Diagram::isVertex(const n0dict& dictionary)
{
	//If we only have 1 particle this isn't a vertex
	if (static_cast<int>(m_externs.size()) == 1) return false;
	else {
		//Count the number of active particles: return early if this is less than 2
		if (std::count_if(m_externs.begin(), m_externs.end(), [](Particle& p) -> bool {return p.isActive(); }) < 2) return false;

		//Check if we have 2 particles
		if (static_cast<int>(m_externs.size()) == 2) {
			//Check if this is a propagator (i.e. a particle and its antiparticle meet)
			if (m_externs[0].getType() == getAntiParticle(m_externs[1].getType())) return true;

			//If it isn't a propagator, this isn't a vertex
			else return false;
		}
		//We now have more than 2 particles
		else {
			//Get the types of the particles
			auto types{ vec2multiset(m_externs) };

			//Search for this interaction in the n->0 dictionary
			for (auto interaction : dictionary) {
				//If we find it, this is a vertex
				if (interaction == types) return true;
			}

			//If it wasn't found, this isn't a vertex
			return false;
		}
	}
}

//Create a process from incoming and outgoing particle names
Process::Process(std::vector<P> incoming, std::vector<P> outgoing) {
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

//Allow for output of a diagram to std::cout
std::ostream& operator<< (std::ostream& out, const Diagram& diag) {
	//Loop through the vertices
	for (auto vertex : diag.getVertices()) {
		//Get particle information
		auto legids{ vertex.getConnectionIDs() };
		auto legtypes{ vertex.getConnectionTypes() };

		//If there are two particles at the vertex, this is a propagator, so display the propagator type and the connected point IDs
		if (legids.size() == 2) {
			std::cout << "\tPropagator | ";
			std::cout << legtypes[0] << " (" << legids[0] << ","<<legids[1]<<"), ";
		}

		//Otherwise this is a vertex, so display the connected point types and their IDs
		else {
			std::cout << "\t    Vertex | ";
			for (int i{}; i < static_cast<int>(legids.size()); ++i) {
				std::cout << legtypes[i] << " (" << legids[i] << "), ";
			}
		}
		std::cout << '\n';
	}

	return out;
}

//Given a diagram and interaction dictionaries, return a vector of all possible completed tree-level diagrams
std::vector<Diagram> connect(Diagram& diag, const n0dict& nto0, const n1dict& nto1, bool debug) {
	//Store the current external vertices of this diagram
	auto externs{ diag.getExterns() };

	//DEBUG: Print details about all the current external particles
	if (debug) {
		std::cout << "Current External Particles\n";
		for (auto part : externs) {
			std::cout << part.getType() << " , " << part.isActive() << " | ";
		}
		std::cout << '\n';
		std::cout << "---------------------------------------------\n";
	}

	//Store the number of external particles
	int s{ static_cast<int>(externs.size()) };

	//Check to see if we can form a vertex
	if (diag.isVertex(nto0)) {
		//Collect all particle information ready to create a vertex
		std::vector<int> idstoadd;
		std::vector<P> typestoadd{};
		for (Particle part : externs) {
			idstoadd.push_back(part.getID());
			typestoadd.push_back(part.getType());
		}

		//DEBUG: Declare this to be a vertex
		if (debug) {
			std::cout << "IS VERTEX : DONE\n";
			std::cout << "=============================================\n";
		}

		//Return a single diagram consisting of the external points and a vertex connecting them
		return std::vector<Diagram> {Diagram(externs, Vertex(idstoadd,typestoadd))};
	}

	//If we don't have a vertex but there are now two or fewer particles left, this process has failed, so return an empty vector
	else if (s<=2) {
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
			for (auto grouping : groupinglist) {
				for (auto group : grouping.first) {
					for (auto part : group) {
						std::cout << part.getType() << ',';
					}
					std::cout << '|';
				}
				for (auto part : grouping.second) {
					std::cout << part.getType() << ',';
				}
				std::cout << '\n';
			}
			std::cout << "---------------------------------------------\n";
		}

		//For each grouping in the list...
		for (pairedgrouping grouping : groupinglist) {
			//...get a list of possible new products for this group
			listofproducts prodlist{ getNewExterns(grouping, nto1) };

			//DEBUG : Display the current grouping and what it will reduce to
			if (debug) {
				std::cout << "Current Grouping: \n\t";
				for (auto group : grouping.first) {
					for (auto part : group) {
						std::cout << part.getType() << ',';
					}
					std::cout << "| ";
				}
				std::cout << " |X| ";
				for (auto part : grouping.second) {
					std::cout << part.getType() << ',';
				}
				std::cout << '\n';
				std::cout << "Product lists: \n";
				for (auto combo : prodlist) {
					std::cout << '\t';
					for (auto group : combo) {
						std::cout << group << "| ";
					}
					std::cout << '\n';
				}
			}

			//Skip this grouping if it doesn't produce any valid interactions
			if ((prodlist.size()==0||prodlist.size()==1)&&(prodlist[0].size()==0)) {
				//DEBUG : Declare the grouping to be invalid
				if (debug) {
					std::cout << "GROUPING INVALID : CONTINUING\n";
					std::cout << "---------------------------------------------\n";
				}

				//Skip
				continue;
			}

			//Go through each possible interaction product
			for (int j{}; j < static_cast<int>(prodlist.size()); ++j) {
				//Set up the new particle and vertex containers
				std::vector<Particle> particlestoadd;
				std::vector<Vertex> verticestoadd;

				//Go through each of the groups that have been formed
				for (int i{}; i < static_cast<int>(grouping.first.size()); ++i) {
					//Set up the new particle information containers for vertex production
					std::vector<int> idstoadd{};
					std::vector<P> typestoadd{};

					//Add the old particles that have "merged"
					for (auto oldpart : grouping.first[i]) {
						idstoadd.push_back(oldpart.getID());
						typestoadd.push_back(oldpart.getType());
					}

					//Add the new (anti)particle to the external container and add it to the vertex
					particlestoadd.push_back(Particle(prodlist[j][i], true));
					idstoadd.push_back(particlestoadd.back().getID());
					typestoadd.push_back(prodlist[j][i]);

					//DEBUG: Print out the vertex that's going to be added
					if (debug) {
						std::cout << "Adding a vertex:\n\t";
						for (int k{}; k < static_cast<int>(idstoadd.size()); ++k) {
							std::cout << typestoadd[k] << ", " << idstoadd[k] << " | ";
						}
						std::cout << '\n';
					}

					//Create and add the vertex to the current diagram
					verticestoadd.push_back(Vertex(idstoadd, typestoadd));
				}

				//Now add the previous external particles to the new container, setting them inactive first
				for (auto ungrouped : grouping.second) {
					ungrouped.setActive(false);
					particlestoadd.push_back(ungrouped);
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
				for (auto sd : connectedsds) {
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