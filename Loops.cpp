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

std::vector<LoopDiagram> connectLoop1PI(const Diagram& diag, const int num_loops, const std::vector<LoopyVertex>& loopyvs) {

	std::vector<LoopDiagram> list_of_diagrams;

	/* ALGORITHM
	*	> Check if this is a loopy vertex: if it is, add this to the possibilities
	*	> Go through all subsets of the legs
	*		> For each subset, go through all possible products which could lead to at least one extra loop
	*			> For each product, work out how many loops it will introduce. Initiate a recursion
	*			> At tree level, we insist that the only possible connection is a single vertex to avoid duplication further down the line
	*	> Bring everything back together
	*/

	std::multiset<P> externparttypes;
	for (const Particle& part : diag.getExterns()) {
		externparttypes.insert(part.getType());
	}
	for (const LoopyVertex& lv : loopyvs) {
		if ((lv.external_particles_in == externparttypes) && (lv.external_particles_out == std::multiset<P>{})) {
			LoopDiagram toadd(diag.getExterns());

		}
	}


	//Get all subsets of the legs of minimum size 1
	//listofpairedgroupings subsets{ getSubsets(,1) };


	return list_of_diagrams;
}

void LoopDiagram::addLoopyVertex(const LoopyVertex& lv, const std::vector<Particle> inbound_parts)
{
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
		Vertex groupvertex(group.size());
		for (const P& ptype : group) {
			for (size_t i{}; i < intparticles.size(); ++i) {
				if (intparticles[i].getType() == ptype) {
					groupvertex.addLegs(std::vector<int>{intparticles[i].getID()}, std::vector<P> {ptype});
					intparticles.erase(intparticles.begin() + i);
					break;
				}
			}
		}
		this->addVertex(groupvertex);
	}
}
