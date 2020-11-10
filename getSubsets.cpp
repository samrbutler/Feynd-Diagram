#include "getSubsets.h"

//Overload the equality operator to allow groupings to be equated
bool operator==(const grouping& part1, const grouping& part2)
{
	//Return false if there are different numbers of groups
	if (part1.size() != part2.size()) return false;

	//Return false if any of the groups are different
	for (size_t i{}; i < part1.size(); ++i) {
		if (part1[i] != part2[i]) return false;
	}

	//Otherwise return true
	return true;
}

//Given a list of particles, return a list of all possible subsets and their complements as a 'listofpairedgroupings'
listofpairedgroupings getSubsets(const std::vector<Particle>& input, const size_t min_size)
{
	//Set up the container
	listofpairedgroupings pairings{};

	//Loop over all binary representations from 0 to 2^(number of particles)-1
	for (int i{}; i < pow(2, input.size()); ++i) {

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

		//We don't care about the subset if it is size 1 or trivial
		if (subset.size() >= min_size) {
			//Create and add the subset to a vector of vectors
			std::vector<std::vector<Particle>> toadd{};
			toadd.push_back(subset);

			//Pair up the subset and its complement
			pairedgrouping pairup{ std::make_pair(toadd, notsubset) };

			//Add it to the return vector
			pairings.push_back(pairup);
		}
	}

	return pairings;
}

listofpairedgroupings getSubsets(const std::vector<Particle>& input, const size_t min_size, const size_t max_size)
{
	listofpairedgroupings to_maximise{ getSubsets(input,min_size) };
	size_t loc{};
	while (loc < to_maximise.size()) {
		size_t num_interactions{ to_maximise[loc].first[0].size() };
		if (num_interactions > max_size) to_maximise.erase(to_maximise.begin() + loc);
		else ++loc;
	}
	return to_maximise;
}