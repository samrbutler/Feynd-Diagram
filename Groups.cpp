/*Groups.cpp :
	Operator overloads
		- bool operator==(const grouping& part1, const grouping& part2)
			> allow groupings to be equated
	Function definitions
		- listofpairedgroupings getSubsets(std::vector<Particle>& input)
			> given a list of particles, return a list of all possible subsets
				and their complements as a 'listofpairedgroupings'
			> generating function: returns by value
		- listofpairedgroupings getGroupings(pairedgrouping& pairup)
			> given a partially completed 'pairedgrouping', return all possible
				completions of the grouping without duplicates
			> recursive function
			> generating function: returns by value
*/

#include "Groups.h"

#include "Model.h"
#include "Particles.h"

#include <algorithm>
#include <utility>
#include <vector>

//Overload the equality operator to allow groupings to be equated
bool operator==(const grouping& part1, const grouping& part2) {

	//Return false if there are different numbers of groups
	if (part1.size() != part2.size()) return false;

	//Return false if any of the groups are different
	for (int i{}; i < static_cast<int>(part1.size()); ++i) {
		if (part1[i] != part2[i]) return false;
	}

	return true;
}

//Given a list of particles, return a list of all possible subsets and their complements as a 'listofpairedgroupings'
listofpairedgroupings getSubsets(const std::vector<Particle>& input) {
	//Set up the container
	listofpairedgroupings pairings{};

	//Loop over all binary representations from 0 to 2^(number of particles)-1
	for (int i{}; i < pow(2, input.size()); ++i) {

		//Set up containers
		std::vector<Particle> subset{};
		std::vector<Particle> notsubset{};

		//Loop over all the bits in i
		for (int j{}; j < static_cast<int>(input.size()); ++j) {
			//If the bit j is a 1, add the jth particle to the subset
			if ((i & (1 << j)) != 0) subset.push_back(input[j]);
			//If not, add it to the ungrouped set
			else notsubset.push_back(input[j]);
		}

		//We don't care about the subset if it is size 1 or trivial
		if (subset.size() >= 2) {
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

//Given a partially completed 'pairedgrouping', return all possible completions of the grouping without duplicates
listofpairedgroupings getGroupings(const pairedgrouping& pairup) {
	//Create the empty vector of groupings
	listofpairedgroupings list{};

	//Extract the current vector of groups
	grouping currentgroups{ pairup.first };

	//Add the current pairing to the list (i.e. we terminate pairing here)
	list.push_back(pairup);

	//Get the vector of ungrouped elements
	const std::vector<Particle>& notPaired{ pairup.second };

	//If we can form another group, do it  
	if (static_cast<int>(notPaired.size()) > 1) {
		//Go through each of the new subsets
		for (const pairedgrouping& pairup : getSubsets(notPaired)) {
			//Initiate recursion to get groupings from unpaired elements
			listofpairedgroupings newgroupings{ getGroupings(pairup) };

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
	//Set up container for duplicates
	listofpairedgroupings nodupes{};

	//Loop over groupings
	for (int i{}; i < static_cast<int>(list.size()); ++i) {
		//Loop over each group in the grouping
		for (int j{}; j < static_cast<int>(list[i].first.size()); ++j) {
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