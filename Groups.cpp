/*Groups.cpp :
*/

#include "Groups.h"

#include "Model.h"
#include "Particles.h"
#include "Utilities.h"

#include <algorithm>
#include <utility>
#include <vector>

//Overload the equality operator to allow groupings to be equated
bool operator==(const grouping& part1, const grouping& part2) {

	//Return false if there are different numbers of groups
	if (part1.size() != part2.size()) return false;

	//Return false if any of the groups are different
	for (size_t i{}; i < part1.size(); ++i) {
		if (part1[i] != part2[i]) return false;
	}

	return true;
}

//Given a list of particles, return a list of all possible subsets and their complements as a 'listofpairedgroupings'
listofpairedgroupings getSubsets(const std::vector<Particle>& input, const size_t min_size) {
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

listofpairedgroupings getSubsets(const std::vector<Particle>& input, const size_t min_size, const size_t max_size) {
	listofpairedgroupings to_maximise{ getSubsets(input,min_size) };
	size_t loc{};
	while (loc < to_maximise.size()) {
		size_t num_interactions{ to_maximise[loc].first[0].size() };
		if (num_interactions > max_size) to_maximise.erase(to_maximise.begin() + loc);
		else ++loc;
	}
	return to_maximise;
}

//Given a partially completed 'pairedgrouping', return all possible completions of the grouping without duplicates
listofpairedgroupings getGroupings(const pairedgrouping& pairup, const int min_size) {
	//Create the empty vector of groupings
	listofpairedgroupings list{};

	//Extract the current vector of groups
	grouping currentgroups{ pairup.first };

	//Add the current pairing to the list (i.e. we terminate pairing here)
	list.push_back(pairup);

	//Get the vector of ungrouped elements
	const std::vector<Particle>& notPaired{ pairup.second };

	//If we can form another group, do it  
	if (notPaired.size() >= min_size) {
		//Go through each of the new subsets
		for (const pairedgrouping& pairup : getSubsets(notPaired,min_size)) {
			//Initiate recursion to get groupings from unpaired elements
			listofpairedgroupings newgroupings{ getGroupings(pairup,min_size) };

			//For each new grouping that is available
			for (pairedgrouping newgroup : newgroupings) {
				//Create an empty container
				grouping toadd{};

				//Reserve the space for efficiency
				toadd.reserve(currentgroups.size() + newgroup.first.size());

				//Insert the current grouping and the new grouping
				toadd.insert(toadd.end(), currentgroups.begin(), currentgroups.end());
				toadd.insert(toadd.end(), newgroup.first.begin(), newgroup.first.end());

				//Add this new grouping to the output
				list.push_back(std::make_pair(toadd, newgroup.second));
			}
		}
	}
	//Set up container
	listofpairedgroupings nodupes{};

	//Loop over groupings
	for (size_t i{}; i < list.size(); ++i) {
		//Loop over each group in the grouping
		for (size_t j{}; j < list[i].first.size(); ++j) {
			//Sort this group
			std::sort(list[i].first[j].begin(), list[i].first[j].end());
		}

		//Sort the overall grouping
		std::sort(list[i].first.begin(), list[i].first.end());

		//This will get set to true if there is a duplicate
		bool isfound{ false };

		//Loop over the groupings we've already found, and set isfound to true if there's a match
		for (const pairedgrouping& dupe : nodupes) {
			if (dupe.first == list[i].first) isfound = true;
		}

		//If we haven't found this grouping yet, add it to the list
		if (!isfound) nodupes.push_back(list[i]);
	}
	return nodupes;
}