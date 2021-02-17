#include "LoopSignatures.h"
#include "IdenticalityDictionary.h"

std::vector<int> getLoopSignature(const std::vector<Particle>& inbound)
{
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

std::vector<int> invertLoopSignature(const std::vector<int>& sig)
{
	std::vector<int> inverted;
	for (const int x : sig) inverted.push_back(-x);
	return inverted;
}

newloopvalues spawnLoops(const std::vector<Particle>& inbound, const int max_loops, const std::vector<LoopyVertex>& loopyvs)
{
	//Set up container
	newloopprofiles new_loops;
	//Get inbound particle types
	std::multiset<P> inbound_types{ vec2multiset(inbound) };

	//Go through all the loopy vertices and add any matches
	for (const LoopyVertex& lv : loopyvs) {
		if (lv.external_particles_in == inbound_types
			&& lv.num_loops + (static_cast<int>(lv.external_particles_out.size()) - 1) <= max_loops
			&& lv.external_particles_out.size() >= 2
			)
		{
			new_loops.push_back(std::make_pair(lv.external_particles_out, lv));
		}
	}

	//Go through all the normal vertices and add any matches
	for (const auto& interaction : Model::n_to_many) {
		if (interaction.first == inbound_types
			&& static_cast<int>(interaction.second.size()) - 1 <= max_loops)
		{
			new_loops.push_back(std::make_pair(interaction.second, LV_null));
		}
	}

	newloopvalues output;

	//Go through each profile in the new list
	for (const auto& profile : new_loops) {

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
				new_particles.push_back(Particle(part, true, std::vector<int>{ LoopDiagram::global_loop_counter }));
				++LoopDiagram::global_loop_counter;
				++counter;
			}
			else {
				//Create the loop_ids for the final output leg
				std::vector<int> loop_ids(num_loops_spawned);
				std::iota(loop_ids.begin(), loop_ids.end(), 1 - LoopDiagram::global_loop_counter);
				new_particles.push_back(Particle(part, true, loop_ids));
			}
		}

		//Get the additional inbound loop momentum signature
		std::vector<int> inbound_loop_signature{ getLoopSignature(inbound) };
		//Add it to each of the new particles, and also sort out the identicality indices to group identical particles
		auto ptr1{ new_particles.begin() };
		while (ptr1 != new_particles.end()) {
			ptr1->addLoop(inbound_loop_signature);
			if (!ptr1->hasIdenticality()) {
				auto ptr2 = ptr1 + 1;
				while (ptr2 != new_particles.end()) {
					if (ptr1->getType() == ptr2->getType()) {
						if (!ptr1->hasIdenticality())
							ptr1->setIdenticality(IdenticalityDictionary::global_identicality_index++);
						ptr2->setIdenticality(ptr1->getIdenticality());
					}
					++ptr2;
				}
			}

			++ptr1;
		}

		//Add the results to the possiblities
		output.push_back(std::make_pair(new_particles, profile.second));

	}

	return output;
}