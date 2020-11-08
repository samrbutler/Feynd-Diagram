/*Loops.cpp : Code relevant to the implementation of loops
*    Function definitions
*/


#include "Loops.h"

#include "Diagram.h"
#include "Model.h"
#include "Interactions.h"
#include "Particles.h"
#include "Utilities.h"

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
	//This function is going to generate new particles that will go out of scope, so copy the static particle ID counter and reset it later
	std::vector<LoopyVertex> toReturn;
	int storage = Point::getStaticID();
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
				num_vertex_loops[i] = static_cast<int>(perm.first[i].size() - 1);
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
	//Reset the static ID counter and return
	Point::setStaticID(storage);
	return toReturn;
}

std::vector<int> getLoopSignature(const std::vector<Particle>& inbound) {
	//Set up container
	std::vector<int> inbound_loop_signature;
	for (const Particle& p : inbound) {
		std::vector<int> this_signature{ p.getLoops() };
		for (const int loop_id : this_signature) {
			std::vector<int>::iterator loc{ std::find(inbound_loop_signature.begin(),inbound_loop_signature.end(), -loop_id) };
			if (loc == inbound_loop_signature.end()) inbound_loop_signature.push_back(loop_id);
			else inbound_loop_signature.erase(loc);
		}
	}
	return inbound_loop_signature;
}

std::vector<int> invertLoopSignature(const std::vector<int>& sig) {
	std::vector<int> inverted;
	for (const int x : sig) inverted.push_back(-x);
	return inverted;
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
			&& static_cast<int>(interaction.second.size()) - 1 <= max_loops) {
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

		//Get the additional inbound loop momentum signature
		std::vector<int> inbound_loop_signature{ getLoopSignature(inbound) };
		//And add it to each of the new particles
		for (Particle& p : new_particles) {
			p.addLoop(inbound_loop_signature);
		}

		//Add the results to the possiblities
		output.push_back(std::make_pair(new_particles, profile.second));

	}

	return output;
}

//TO DO : Add arguments for non-model dictionaries
std::vector<LoopDiagram> connect1PI(const LoopDiagram& diag, const int num_loops,
	const std::vector<LoopyVertex>& loopyvs, const n0dict& nto0, const n1dict& nto1, bool debug) {

	std::vector<Particle> external_particles{ diag.getExterns() };
	std::vector<LoopDiagram> list_of_diagrams;

	/* ALGORITHM - we take a vector of m external particles and return all *specific* 1PI completions at num_loops
	*   > Declare (without loss of generality since this is 1PI) the first external particle to be the starting point of the first loop
	*   > Go through all the 1->n vertices, spawning (n-1) new loops
	*		- If connection fails, skip this vertex. Otherwise, add the result to the output list
	*	> Now consider pairing the first external particle to all other inbound particles in turn and go through the 2->n vertices.
	*		- Repeat the above subalgorithm with the new subgraphs
	*	> Keep going until the first external particle is being grouped with
	*		A) (m-2) of the remaining external particles (i.e. after introducing loops they have somewhere to connect to)
	*	 or B) (max_int-2) others for max_int the largest number of particles that can simultaneously interact at a vertex
	*	> If condition A was met, now check if this could be a loopy vertex (these have been generated in advance in generic form)
	*/

	//Take the first particle to be the reference point
	const Particle& loop_start{ external_particles[0] };

	//Collect the other particles in a vector: we can select from these to combine with the reference particle
	const std::vector<Particle> particles_available(external_particles.begin() + 1, external_particles.end());

	//Choose how many more particles are going to combine before spawning the loop
	for (int i{ 0 }; i <= std::min(static_cast<int>(external_particles.size()) - 2, Model::max_legs - 2); ++i) {
		//Get the list of new particles that are going to combine
		listofpairedgroupings choices{ getSubsets(particles_available, i,i) };
		//For each choice:
		for (const pairedgrouping& pg : choices) {
			//Set up the list of particles to combine
			std::vector<Particle> to_combine{ loop_start };
			to_combine.insert(to_combine.end(), pg.first[0].begin(), pg.first[0].end());

			//Set up the list of particles that will be left over
			std::vector<Particle> particles_available_after_grouping{ pg.second };

			//Spawn the new particles
			newloopvalues new_particle_data{ spawnLoops(std::vector<Particle> {to_combine}, num_loops) };

			//For each "spawned loop":
			for (std::pair<std::vector<Particle>, LoopyVertex>& new_set : new_particle_data) {
				//Start to create the vertex particle list
				std::vector<Particle> new_vertex{ to_combine };
				//Work out how many loops are left
				int remaining_loops = num_loops - static_cast<int>(new_set.first.size()) + 1;
				if (!new_set.second.is_null) remaining_loops -= new_set.second.num_loops;
				//Set up the new external particle container for the subdiagram
				std::vector<Particle> new_externs{ particles_available_after_grouping };
				//For each particle spawned:
				for (Particle& p : new_set.first) {
					//Toggle to the antiparticle and add it to the vertex
					new_vertex.push_back(p.toggleAntiPart());
					//Toggle back to the particle and add it to the external container
					new_externs.push_back(p.toggleAntiPart());
				}

				//Create the base diagram and add the distinguished (loopy) vertex
				LoopDiagram base_diagram(external_particles);
				if (new_set.second.is_null) base_diagram.addVertex(Vertex(new_vertex));
				else base_diagram.addLoopyVertex(new_set.second, new_vertex);

				LoopDiagram base_subdiagram(new_externs);
				//Connect the subdiagram

				std::vector<LoopDiagram> subdiagrams{ connectSubdiagram(base_subdiagram, remaining_loops, loopyvs, nto0, nto1, debug) };
				for (LoopDiagram& ld : subdiagrams) {
					LoopDiagram to_add(base_diagram);
					to_add.addVertices(ld.getVertices());
					list_of_diagrams.push_back(to_add);
				}
			}
		}
	}

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

std::vector<LoopDiagram> connectSubdiagram(LoopDiagram& diag, const int num_loops, const std::vector<LoopyVertex>& loopyvs,
	const n0dict& nto0, const n1dict& nto1, const bool debug) {
	/* ALGORITHM
	*	> Check the loop status of the remaining particles: if none of the particles are carrying loop momentum or
	*	  the loop momenta do not cancel out then we have failed
	*	> Check if the remaining particles can connect directly into a simple vertex or 1PI diagram with at least
	*	  two active particles and the required number of loops. Add all possible successful connections to the list
	*	> If there are two particles and they haven't connected above, connection has failed, so return early.
	*	> Otherwise, get all possible groupings of the external particles
	*		> For each grouping, get all possible vertex-level products and work out their loop momenta
	*			> If each group does not include at least one active particle, skip this grouping
	*			> Go through all possible products and get the new external particles.
	*				> Set these new particles active, the other particles passive and make a subdiagram
	*				> Use recursion to connect the subdiagram
	*					> If the subdiagram returned connections, combine the subdiagram into the current diagram and add it to the list
	*					> If the subdiagram fails to connect, go to the next product
	*			> If all products failed to produce a connected diagram, continue
	*		> For each grouping, go through every particle in the theory, and see if we can generate
	*		  a 1PI diagram to get to this target particle
	*			- This requires us to generate groupings of size 1
	*
	*/

	const std::vector<Particle>& externs{ diag.getExterns() };

	//If the loop momenta will not cancel out, return early
	if (getLoopSignature(externs).size() != 0) return {};

	//If no loop momenta are present at all, we have failed
	bool loops_exist{ false };
	for (const Particle& p : externs) {
		if (p.getLoops().size() != 0) loops_exist = true;
	}
	if (!loops_exist) return {};

	std::vector<LoopDiagram> returnvec;

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

	//Check to see if we can use a slightly modified standard connection algorithm
	if (num_loops == 0) {
		//DEBUG: Declare this to be a vertex
		if (debug) {
			std::cout << "No loops remaining\n";
		}

		std::vector<LoopDiagram> zero_loop_completions{ connectZero(diag,nto0,nto1,debug) };
		//Extend these diagrams to loop diagrams and add them to the output
		for (const LoopDiagram& d : zero_loop_completions) returnvec.push_back(d);
		return returnvec;
	}
	std::vector<LoopDiagram> direct_completions{ connect1PI(diag,num_loops,loopyvs,nto0,nto1,debug) };
	if (debug) std::cout << "Found " << direct_completions.size() << " 1PI completions of this process.\n";
	returnvec.insert(returnvec.end(), direct_completions.begin(), direct_completions.end());

	//Group up particles in groups of size >= 1
	pairedgrouping input{ std::make_pair(grouping{},externs) };
	listofpairedgroupings groupinglist{ getGroupings(input,1) };

	//Go through all groupings
	for (const pairedgrouping& pg : groupinglist) {
		//If group size >=2: look for (possibly loopy) ways of producing 1 additional particle.
		


		//For every grouping go through every particle in the theory
		//	> Determine all 1PI subdiagrams to transition the group to the new particle at all loop orders between 1 and num_loops
		//Connect the subdiagrams at the appropriate loop order

	}

	return returnvec;
}

std::vector<LoopDiagram> connectZero(LoopDiagram& diag, const n0dict& nto0, const n1dict& nto1, const bool debug) {

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

	//Create empty return vector to add diagrams to
	std::vector<LoopDiagram> returnvec;

	//Check to see if we can form a vertex - we've already checked that the momenta cancel out
	if (diag.isVertex(nto0)) {
		//DEBUG: Declare this to be a vertex
		if (debug) {
			std::cout << "IS VERTEX : DONE\n";
			std::cout << "=============================================\n";
		}

		//Add a diagram consisting of the external points and a vertex connecting them
		returnvec.emplace_back(LoopDiagram(externs, Vertex(externs)));
		return returnvec;
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
				std::vector<Particle> vertexparts{ grp.first[i] };
				std::vector<int> outbound_signature{ invertLoopSignature(getLoopSignature(vertexparts)) };
				//Add the new (anti)particle to the external container and add it to the vertex
				Particle p_to_add(prodlist[j][i], true, outbound_signature);
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
			LoopDiagram subdiag{ particlestoadd };

			//Initiate a recursion by connecting the subdiagram
			std::vector<LoopDiagram> connectedsds{ connectSubdiagram(subdiag, 0, Model::loopy_vertices, nto0, nto1, debug) };

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
			for (LoopDiagram& sd : connectedsds) {
				LoopDiagram newdiag(diag);
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