/* Former code from connectLoopDiagram 
	for (pairedgroup& pg : groupinglist) {
		//Get the number of groups in the grouping
		int num_groups{ static_cast<int>(pg.first.size()) };

		//Set up containers to store all possible vertices, loopy vertices and 1PI completions for a group
		using lprods = std::vector<std::pair<Particle, LoopyVertex>>;
		using opiprods = std::vector<std::pair<std::pair<Particle, LoopDiagram>, int>>;
		std::vector<std::vector<Particle>> tree_grouping_products;
		std::vector<lprods> loopy_grouping_products;
		std::vector<opiprods> opi_grouping_products;

		//Go through each group and fill in the containers
		for (std::vector<Particle>& g : pg.first) {
			//Get the outbound loop signature
			std::vector<int> loopsig{ invertLoopSignature(getLoopSignature(g)) };

			std::vector<P> product_types{ getProducts(g, nto1) };
			std::vector<Particle> product_particles;
			for (P p : product_types) product_particles.push_back(Particle(p, true, loopsig));
			//Get tree-level products using the nto1 dictionary
			tree_grouping_products.push_back(product_particles);

			//Get loopyvertex products using the loopyvertex dictionary
			lprods loopy_products;
			if (g.size() < Model::max_legs) {
				//Go through every possible output product we might expect
				for (P inbound_prod_type : Model::all_particle_types) {
					//Get all the external particle types
					std::multiset<P> externparttypes{ inbound_prod_type };
					for (const Particle& part : g) {
						externparttypes.insert(part.getType());
					}
					//Go through each loopy vertex
					for (const LoopyVertex& lv : loopyvs) {
						//If the vertex produces external particles or adds the wrong number of loops, skip it
						if ((lv.external_particles_out != std::multiset<P>{}) || (lv.num_loops != num_loops)) continue;
						//Otherwise, check that the external particles match and add to the list
						if (lv.external_particles_in == externparttypes) {
							loopy_products.push_back(std::make_pair(Particle(getAntiParticle(inbound_prod_type), true, loopsig), lv));
						}
					}
				}
			}
			//Add the possibilities to the list
			loopy_grouping_products.push_back(loopy_products);

			//Get the 1PI competions
			opiprods opi_products;
			//Go through every external particle
			for (P inbound_prod_type : Model::all_particle_types) {
				for (int nl{}; nl <= num_loops; ++nl) {
					std::vector<Particle> externs = g;
					externs.push_back(Particle(getAntiParticle(inbound_prod_type)));
					std::vector<LoopDiagram> opicompletions{ connect1PI(LoopDiagram(externs),nl) };
					for (LoopDiagram& ld : opicompletions) {
						opi_products.push_back(std::make_pair(std::make_pair(
							Particle(getAntiParticle(inbound_prod_type), true, loopsig), ld), nl));
					}
				}
			}
			opi_grouping_products.push_back(opi_products);
		}

		//Now choose which option we use for each group
		for (int x{}; x < pow(3, num_groups); ++x) {
			int* signature{ new int[num_groups] {} };
			int y = x;
			int index{};
			while (y > 0) {
				signature[index++] = y % 3;
				y /= 3;
			}
			bool is_valid{ true };
			for (int g{}; g < num_groups; ++g) {
				if (signature[g] == 0) {
					//Use tree-level for this group
					if (tree_grouping_products[g].size() == 0) {
						is_valid = false;
						break;
					}
				}
				else if (signature[g] == 1) {
					if (loopy_grouping_products[g].size() == 0) {
						is_valid = false;
						break;
					}
					//Use loopy-vertex for this group
				}
				else {
					if (opi_grouping_products[g].size() == 0) {
						is_valid = false;
						break;
					}
					//Use 1PI competion for this group
				}
			}
			if (!is_valid) continue;


		}



	}

	return returnvec;
}

*/



/*
Identical grouping removal:

		//Create a set to keep track of "grouping" signatures
		std::set<std::map<int, int>> signature_dictionary;
		//Loop through all combinations until we've considered all options
		int loc{};
		while (loc < choices.subsets.size()) {
			//Set up the signature for this subset
			std::map<int, int> current_signature;
			//Go through all particles that are set to group
			for (const Particle& p : choices.subsets[loc].first) {
				//If the particle has an identicality condition, increase the number of occurrences in the signature
				if (p.hasIdenticality()) {
					++current_signature[p.getIdenticality()];
				}
				//If not, then it is a distinguished particle: add the negative of its ID (all IDs are strictly positive)
				else {
					++current_signature[-p.getID()];
				}
			}
			//If we haven't seen this signature before, this is a physically distinct grouping, so we leave it be
			if (signature_dictionary.find(current_signature) == signature_dictionary.end()) {
				signature_dictionary.insert(current_signature);
				++loc;
			}
			//If we have, this grouping is already included, so remove this copy ([[loc]] is now pointing to the next element)
			else {
				choices.subsets.erase(choices.subsets.begin() + loc);
			}
		}


*/