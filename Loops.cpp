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

int global_loop_counter{ 1 };

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



newloopvalues spawnLoops(const std::vector<Particle>& inbound, const int max_loops, const std::vector<LoopyVertex>& loopyvs) {

	//Set up container
	newloopprofiles new_loops;
	//Get inbound particle types
	std::multiset<P> inbound_types{ vec2multiset(inbound) };

	//Go through all the loopy vertices and add any matches
	for (const LoopyVertex& lv : loopyvs) {
		if (lv.external_particles_in == inbound_types
			&& lv.num_loops + (static_cast<int>(lv.external_particles_out.size()) - 1) <= max_loops
			&& lv.external_particles_out.size() >= 2
			) {
			new_loops.push_back(std::make_pair(lv.external_particles_out, lv));
		}
	}

	//Go through all the normal vertices and add any matches
	for (auto interaction : Model::n_to_many) {
		if (interaction.first == inbound_types
			&& static_cast<int>(interaction.second.size()) - 1 <= max_loops
			) {
			new_loops.push_back(std::make_pair(interaction.second, LV_null));
		}
	}

	newloopvalues output;

	//Go through each profile in the new list
	for (auto profile : new_loops) {

		//Get the number of spawned loops
		int num_loops_spawned{ static_cast<int>(profile.first.size()) - 1 };
		if (!profile.second.is_null) num_loops_spawned += profile.second.num_loops;
		//Create the new particle container
		std::vector<Particle> new_particles;
		//Set up the rolling counter for loop momentum
		int counter{ 1 };

		//Go through each particle in the profile and add a new particle with the correct loop momenta
		for (P part : profile.first) {
			if (counter != num_loops_spawned + 1) {
				new_particles.push_back(Particle(part, true, std::vector<int>{ global_loop_counter }));
				++global_loop_counter;
				++counter;
			}
			else {
				//Create the loop_ids for the final output leg
				std::vector<int> loop_ids(num_loops_spawned);
				std::iota(loop_ids.begin(), loop_ids.end(), 1 - global_loop_counter);
				new_particles.push_back(Particle(part, true, loop_ids));
			}
		}
		//Add this to the possiblities
		output.push_back(std::make_pair(new_particles, profile.second));

	}

	return output;
}

//TO DO : Add arguments for non-model dictionaries
std::vector<LoopDiagram> connect1PI(const std::vector<Particle>& external_particles, const int num_loops, const std::vector<LoopyVertex>& loopyvs) {

	int remaining_loops{ num_loops };
	std::vector<LoopDiagram> list_of_diagrams;

	/* ALGORITHM - we take a vector of m external particles and return all *specific* 1PI completions at num_loops
	*			 - an X in the margin indicates the corresponding portion has been implemented
	*X  > Declare (without loss of generality since this is 1PI) the first external particle to be the starting point of the first loop
	*X  > Go through all the 1->n vertices, spawning (n-1) new loops
	*X		- Add the appropriate loop momenta to a tracking vector (use + and - to indicate direction and a static id number to indicate value)
	*		- Complete the resulting diagrams using an appropriate combination of 1PI loop diagrams of lesser loop order
	*			= Convert n->0 loop topologies to n->1 topologies
	*			= Use connection algorithm as before with additional loopy topologies and allowances for 1->1 loopy interactions
	*			= Additional restriction is that legs carrying loop momenta cannot connect to external particles - when spawning a new
	*			  leg from an n->1 interaction work out what momenta the new leg carries
	*		- If connection fails, skip this vertex. Otherwise, add the result to the output list
	*X	> Now consider pairing the first external particle to all other inbound particles in turn and go through the 2->n vertices.
			- Repeat the above subalgorithm with the new subgraphs
	*	> Keep going until the first external particle is being grouped with
	*		A) (m-1) of the remaining external particles (i.e. after introducing loops they have somewhere to connect to)
	*	 or B) (max_int-2) others for max_int the largest number of particles that can simultaneously interact at a vertex
	*X	> If condition A was met, now check if this could be a loopy vertex (these have been generated in advance in generic form)
	*/

	const int m{ static_cast<int>(external_particles.size()) };

	const Particle& loop_start{ external_particles[0] };
	const P start_type{ loop_start.getType() };

	const std::vector<Particle> particles_available(external_particles.begin() + 1, external_particles.end());

	newloopvalues new_particle_data{ spawnLoops(std::vector<Particle> {loop_start}, remaining_loops) };

	//CHECK THAT LOOPS ARE BEING SPAWNED CORRECTLY
	for (auto loopvalue : new_particle_data) {
		for (const Particle& part : loopvalue.first) {
			std::cout << part.getType() << " (" << part.getID() << ") carrying momenta: ";
			for (int loop : part.getLoops()) {
				std::cout << loop << " , ";
			}
			std::cout << '\n';
		}
		std::cout << "\n===============================\n";
	}
	std::cout << '\n';
	//

	for (std::pair<std::vector<Particle>, LoopyVertex>& new_set : new_particle_data) {
		//Add the antiparticles of new_set to loop_start and connect to the (possibly loopy) vertex
		//Add all the new particles to a subdiagram and call the loopy version of connect with the appropriate number of loops
		std::vector<Particle> new_vertex{ loop_start };
		std::vector<Particle> new_externs{ particles_available };
		for (Particle& p : new_set.first) {
			new_vertex.push_back(p.toggleAntiPart());
			p.toggleAntiPart();
			new_externs.push_back(p);
		}
		/*
		std::vector<LoopDiagram> subdiagrams{ connectFull(~new_externs~) };  <========== Still need to implement connectFull
		for (LoopDiagram& ld : subdiagrams)
		if (new_set.second.is_null) {
			ld.addVertex(Vertex(new_vertex));
		}
		else {
			ld.addLoopyVertex(new_set.second, new_vertex);
		}
		*/
	}


	/*
	//Choose how many more particles are going to combine before spawning the loop
	for (int i{ 1 }; i <= std::min(m - 1, Model::max_legs - 2); ++i) {
		listofpairedgroupings choices{ getSubsets(particles_available, i,i) };
		for (const pairedgrouping& pg : choices) {
			for (const Particle& part : pg.first[0]) {
				std::cout << part.getType() << " (" << part.getID() << ") , ";
			}
			std::cout << '\n';
		}
	}
	*/

	if (external_particles.size() <= Model::max_legs) {
		//Get the external particle types
		std::multiset<P> externparttypes;
		for (const Particle& part : external_particles) {
			externparttypes.insert(part.getType());
		}
		//Go through each loopy vertex
		for (const LoopyVertex& lv : loopyvs) {
			//If the vertex produces external particles or adds the wrong number of loops, skip it
			if ((lv.external_particles_out != std::multiset<P>{}) || (lv.num_loops != num_loops)) continue;
			//Otherwise, check that the external particles match and add a diagram with the loopyvertex
			if (lv.external_particles_in == externparttypes) {
				LoopDiagram toadd(external_particles);
				toadd.addLoopyVertex(lv, external_particles);
				list_of_diagrams.push_back(toadd);
			}
		}
	}

	return list_of_diagrams;
}