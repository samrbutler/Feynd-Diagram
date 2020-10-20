/*Loops.cpp : Code relevant to the implementation of loops
*    Function definitions
*/


#include "Loops.h"

#include "Diagram.h"
#include "Model.h"
#include "Interactions.h"
#include "Particles.h"

#include <numeric>
#include <set>
#include <vector>

void LoopDiagram::addLoopyVertex(const LoopyVertex& lv, const std::vector<Particle>& inbound_parts) {
	//Generate the internal particles
	std::vector<Particle> intparticles;
	std::vector<Particle> allparticles{ inbound_parts };
	for (const P& intpart : lv.internal_particles) {
		intparticles.push_back(Particle(intpart));
	}
	allparticles.insert(allparticles.end(), intparticles.begin(), intparticles.end());
	//Generate and add the central vertex
	this->addVertex(Vertex(allparticles));
	//Generate and add the internal vertices
	for (const std::multiset<P>& group : lv.internal_connections) {
		std::vector<Particle> toadd;
		for (const P& ptype : group) {
			for (size_t i{}; i < intparticles.size(); ++i) {
				if (intparticles[i].getType() == ptype) {
					toadd.push_back(intparticles[i]);
					intparticles.erase(intparticles.begin() + i);
					break;
				}
			}
		}
		this->addVertex(Vertex(toadd));
	}
}

bool operator==(const LoopyVertex& lv1, const LoopyVertex& lv2) {
	if (lv1.external_particles_in != lv2.external_particles_in) return false;
	else if (lv1.external_particles_out != lv2.external_particles_out) return false;
	else if (lv1.internal_connections != lv2.internal_connections) return false;
	else return true;
}

std::vector<LoopyVertex> getLoopyVertices(const int max_loops, const loopdict& dictionary) {

	std::vector<LoopyVertex> toReturn;

	for (const std::pair<std::multiset<P>, std::multiset<P>>& vertex : dictionary) {

		//Get the multiset of particle types that we need to group
		const std::multiset<P>& toGroup{ vertex.second };
		//Get the multiset of inbound particles
		const std::multiset<P>& inbound{ vertex.first };
		//Convert the multiset of particle types into a vector of particles
		std::vector<Particle> particles;
		for (const P& particlename : toGroup) {
			particles.push_back(Particle(particlename));
		}
		//Get all the groupings
		listofpairedgroupings legpermutations{
			getGroupings(std::make_pair(std::vector<std::vector<Particle>> {}, std::vector<Particle>{particles})) };

		//Go through each grouping of the vertex legs
		for (const pairedgrouping& perm : legpermutations) {

			//Get the number of groups
			size_t num_groups{ perm.first.size() };
			//Work out how many loops this combination is going to add
			int* num_vertex_loops{ new int[num_groups] {} };
			for (size_t i{}; i < num_groups; ++i) {
				num_vertex_loops[i] = perm.first[i].size() - 1;
			}
			int total_loops{ std::accumulate(num_vertex_loops, num_vertex_loops + num_groups, 0) };

			delete[] num_vertex_loops;

			//If we will add more loops than we can afford to add, or no loops at all, skip
			if ((total_loops > max_loops) || (total_loops == 0)) continue;

			//Go through and check that this grouping is valid, i.e. we can attach a vertex. If we can't, skip it
			bool isValid{ true };
			for (const std::vector<Particle>& group : perm.first) {
				Diagram* diag{ new Diagram(group) };
				if (!diag->isVertex()) {
					isValid = false;
					delete diag;
					break;
				}
				delete diag;
			}
			if (!isValid) continue;

			//Since everything is OK, make the containers
			std::multiset<P> extern_in;
			std::multiset<P> extern_out;
			std::multiset<P> interns;
			std::multiset<std::multiset<P>> intern_conns;

			//Add the inbound particles and the remaining outbound particles
			extern_in.insert(inbound.begin(), inbound.end());
			for (const Particle& part : perm.second) {
				extern_out.insert(part.getType());
			}
			//For each group in the permutation, add the appropriate vertex
			for (const std::vector<Particle>& group : perm.first) {
				std::multiset<P> legs;
				for (const Particle& part : group) {
					legs.insert(part.getType());
					interns.insert(part.getType());
				}
				intern_conns.insert(legs);
			}
			//Check if we've already added this
			bool isFound{ false };
			LoopyVertex lvtoadd{ extern_in, extern_out, interns, intern_conns, total_loops };
			for (const LoopyVertex& lv : toReturn) {
				if (lv == lvtoadd) {
					isFound = true;
					break;
				}
			}
			//If not, add it
			if (!isFound)
				toReturn.push_back(lvtoadd);
		}
	}
	return toReturn;
}

//TO DO : Add arguments for non-model dictionaries
std::vector<LoopDiagram> connectLoop1PI(const LoopDiagram& diag, const int num_loops, const std::vector<LoopyVertex>& loopyvs) {

	std::vector<LoopDiagram> list_of_diagrams;

	/* ALGORITHM - we take a disconnected diagram with m external particles and return all *specific* 1PI completions at num_loops
	*   > Declare (without loss of generality since this is 1PI) the first external particle to be the starting point of the first loop
	*	> Go through all the 1->n vertices, spawning (n-1) new loops
	*		- Add the appropriate loop momenta to a tracking vector (use + and - to indicate direction and a static id number to indicate value)
	*		- Complete the resulting diagrams using an appropriate combination of (already found) 1PI loop diagrams of lesser loop order
	*			= Convert n->0 loop topologies to n->1 topologies
	*			= Use connection algorithm as before with additional topologies and allowances for 1->1 loopy interactions
	*			= Additional restriction is that legs carrying loop momenta cannot connect to external particles - when spawning a new
	*			  leg from an n->1 interaction work out what momenta the new leg carries
	*		- If connection fails, skip this vertex. Otherwise, add the result to the output list
	*	> Now consider pairing the first external particle to all other inbound particles in turn and go through the 2->n vertices. Repeat the
	*	  above subalgorithm with the new subgraphs
	*	> Keep going until the first external particle is being grouped with
	*		A) (m-1) of the remaining external particles (i.e. after introducing loops they have somewhere to connect to)
	*	 or B) (max_int-2) others for max_int the largest number of particles that can simultaneously interact at a vertex
	*	> If condition A was met, now check if this could be a loopy vertex (these have been generated in advance in generic form)
	*/

	//Get the external particle types
	std::multiset<P> externparttypes;
	for (const Particle& part : diag.getExterns()) {
		externparttypes.insert(part.getType());
	}
	//Go through each loopy vertex
	for (const LoopyVertex& lv : loopyvs) {
		//If the vertex produces external particles or adds the wrong number of loops, skip it
		if ((lv.external_particles_out != std::multiset<P>{}) || (lv.num_loops != num_loops)) continue;
		//Otherwise, check that the external particles match and add a diagram with the loopyvertex
		if (lv.external_particles_in == externparttypes) {
			LoopDiagram toadd(diag.getExterns());
			toadd.addLoopyVertex(lv, diag.getExterns());
			list_of_diagrams.push_back(toadd);
		}
	}

	//Get all subsets of the external particles of minimum size 1
	listofpairedgroupings subsets{ getSubsets(diag.getExterns(),1) };
	//Go through all subsets
	for (const pairedgrouping& subsetcontainer : subsets) {
		//Store the list of interacting particles
		const std::vector<Particle>& subset{ subsetcontainer.first[0] };
		//And store the list of non-interacting particles
		const std::vector<Particle>& notsubset{ subsetcontainer.second };
		//Get their types
		std::multiset<P> subset_types{ vec2multiset(subset) };
		
		//Search for an appropriate interaction in the n_to_many dictionary
		for (const std::pair<std::multiset<P>, std::multiset<P>>& interaction : Model::n_to_many) {
			//Store the list of admitted particles
			const std::multiset<P>& inbound{ interaction.first };
			//If they don't match what we have, continue to the next interaction
			if (inbound != subset_types) continue;
			//Work out how many loops this will add
			const int loops_added{ interaction.second.size() - 1 };
			//If it adds too many, skip it
			if (loops_added > num_loops) continue;
			//Get the product particle types
			const std::multiset<P>& outbound{ interaction.second };
			//Collect the outbound particles
			std::vector<Particle> outboundparticles{ notsubset };
		}

		for (int loops_at_vertex{}; loops_at_vertex < num_loops - 1; ++loops_at_vertex) {
		}
		
	}


	return list_of_diagrams;
}