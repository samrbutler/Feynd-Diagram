#include "Subsets.h"

SubsetList::SubsetList(const std::vector<Particle>& input, const size_t min_size, const size_t max_size)
{
	//Loop over all binary representations from 0 to 2^(number of particles)-1
	for (int i{}; i < pow(2, input.size()); ++i) {

		if (numBitsSet(i) > max_size) continue;
		//Set up containers
		std::vector<Particle> subset{};
		std::vector<Particle> notsubset{};

		//Loop over all the bits in i
		for (size_t j{}; j < input.size(); ++j) {
			//If the bit j is a 1, add the jth particle to the subset
			if ((i & (1 << j)) != 0) subset.push_back(input[j]);
			//If not, add it to the ungrouped set
			else notsubset.push_back(input[j]);
		}
		//We don't care about the subset if it isn't big enough
		if (subset.size() >= min_size) {
			group toadd{};
			toadd.push_back(subset);
			//Add it to the storage vectors
			subsets.push_back(std::make_pair(subset, notsubset));
			pairedgroup pg = std::make_pair(toadd, notsubset);
			subsets_as_groups.push_back(pg);
		}
	}
}
