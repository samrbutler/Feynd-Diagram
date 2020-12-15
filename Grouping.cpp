#include "Grouping.h"

//Overload the equality operator to allow groupings to be equated
bool operator==(const group& part1, const group& part2)
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

listofpairedgroups Grouping::getGroupings(const pairedgroup& pairup, const int min_size)
{
	//Create the empty vector of groupings
	listofpairedgroups list{};

	//Extract the current vector of groups
	group currentgroups{ pairup.first };

	//Add the current pairing to the list (i.e. we terminate pairing here)
	list.push_back(pairup);

	//Get the vector of ungrouped elements
	const std::vector<Particle>& notPaired{ pairup.second };

	//If we can form another group, do it  
	if (notPaired.size() >= min_size) {
		//Get the new subsets of the unpaired group
		SubsetList unpaired(notPaired, min_size);
		//Go through each of the new subsets
		for (const pairedgroup& pairup : unpaired.subsets_as_groups) {
			//Initiate recursion to get groupings from unpaired elements
			listofpairedgroups newgroupings{ getGroupings(pairup,min_size) };

			//For each new grouping that is available
			for (pairedgroup newgroup : newgroupings) {
				//Create an empty container
				group toadd{};

				//Reserve the space for efficiency
				toadd.reserve(currentgroups.size() + newgroup.first.size());

				//Insert the current grouping and the new grouping
				toadd.insert(toadd.end(), currentgroups.begin(), currentgroups.end());
				toadd.insert(toadd.end(), newgroup.first.begin(), newgroup.first.end());

				pairedgroup pg {std::make_pair(toadd, newgroup.second)};

				for (size_t j{}; j < pg.first.size(); ++j) {
					//Sort this group
					std::sort(pg.first[j].begin(), pg.first[j].end());
				}

				//Sort the overall grouping
				std::sort(pg.first.begin(), pg.first.end());

				//Add this new grouping to the output
				list.push_back(pg);
			}
		}
	}

	//Set up container
	listofpairedgroups nodupes{};

	//Loop over groupings
	for (size_t i{}; i < list.size(); ++i) {

		//This will get set to true if there is a duplicate
		bool isfound{ false };

		//Loop over the groupings we've already found, and set isfound to true if there's a match
		for (const pairedgroup& dupe : nodupes) {
			if (dupe.first == list[i].first) isfound = true;
		}

		//If we haven't found this grouping yet, add it to the list
		if (!isfound) nodupes.push_back(list[i]);
	}
	return nodupes;
}


//Temporary function: this will be removed
std::vector<pairedgroup> Grouping::getSubsets(const std::vector<Particle>& input, const size_t min_size,
	const size_t max_size)
{
	//Set up the container
	std::vector<pairedgroup> pairings{};

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

		//We don't care about the subset if it is size 1 or trivial
		if (subset.size() >= min_size) {
			//Create and add the subset to a vector of vectors
			std::vector<std::vector<Particle>> toadd{};
			toadd.push_back(subset);

			//Pair up the subset and its complement
			pairedgroup pairup{ std::make_pair(toadd, notsubset) };

			//Add it to the return vector
			pairings.push_back(pairup);
		}
	}

	return pairings;
}

Grouping::Grouping(const std::vector<Particle>& particles, const int min_size)
{
	possible_groupings = getGroupings(std::make_pair(group{}, particles), min_size);
}
