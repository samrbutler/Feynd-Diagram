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
				}
				intern_conns.insert(legs);
			}
			//Check if we've already added this
			bool isFound{ false };
			LoopyVertex lvtoadd{ extern_in, extern_out, intern_conns, total_loops };
			for (const LoopyVertex& lv : toReturn) {
				if (lv == lvtoadd) {
					isFound = true;
					break;
				}
			}
			//If not, add it
			if (!isFound)
				toReturn.push_back(LoopyVertex{ extern_in, extern_out, intern_conns, total_loops });
		}
	}
	return toReturn;
}

std::vector<Diagram> getLoopTopologies(const std::vector<Particle>& legs, const int num_loops) {

	std::vector<Diagram> list_of_diagrams;
	
	/* ALGORITHM
	*	> Check if this is a loopy vertex: if it is, add this to the possibilities
	*	> Go through all subsets of the legs
	*		> For each subset, go through all possible products which could lead to at least one extra loop
	*			> For each product, work out how many loops it will introduce. Initiate a recursion
	*			> At tree level, we insist that the only possible connection is a single vertex to avoid duplication further down the line
	*	> Bring everything back together
	*/

	//Get all subsets of the legs of minimum size 1
	listofpairedgroupings subsets{ getSubsets(legs,1) };

	return list_of_diagrams;
}
