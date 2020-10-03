// Groups.cpp : Code to implement group finding in lists of Points
#include "Groups.h"

#include "Interactions.h"
#include "Particles.h"

#include <algorithm>
#include <functional>
#include <utility>
#include <vector>

//Overload operator < to sort particle vectors by their first elements
bool operator<(const std::vector<Particle>& vec1, const std::vector<Particle>& vec2) {
	return (vec1[0].getID() < vec2[0].getID());
}

//Overload inequality operator for particle vectors
bool operator!=(const std::vector<Particle>& part1, const std::vector<Particle>& part2) {
	if (part1.size() != part2.size())
		return true;
	for (int i{}; i < static_cast<int>(part1.size()); ++i) {
		if (part1[i] != part2[i])
			return true;
	}
	return false;
}

//Overload equality operator for groupings
bool operator==(const grouping& part1, const grouping& part2) {
	if (part1.size() != part2.size())
		return false;
	for (int i{}; i < static_cast<int>(part1.size()); ++i) {
		if (part1[i] != part2[i])
			return false;
	}
	return true;
}

//Given a vector of particles, produce all suitable subsets
listofpairedgroupings getSubsets(std::vector<Particle>& input) {
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
			if ((i & (1 << j)) != 0)
				subset.push_back(input[j]);
			//If not, add it to the ungrouped set
			else
				notsubset.push_back(input[j]);
		}
		//We don't care about the subset if it is size 1 or trivial
		if (subset.size() >= 2) {
			//Create and add the subset to a vector of vectors
			std::vector<std::vector<Particle>> toadd{};
			toadd.push_back(subset);
			//Pair up the subset and its complement
			auto pairup{ std::make_pair(toadd, notsubset) };
			//Add it to the return vector
			pairings.push_back(pairup);
		}
	}
	return pairings;
}

//Given a paired grouping, extract and append more groups from the presently ungrouped elements
//NOTE: This is a recursive function
listofpairedgroupings getGroupings(pairedgrouping& pairup) {
	//Create the empty vector of groupings
	listofpairedgroupings list{};
	//Extract the current vector of groups
	auto currentgroups{ pairup.first };
	//Add the current pairing to the list (i.e. we terminate pairing here)
	list.push_back(pairup);
	//Get the vector of ungrouped elements
	auto notPaired{ pairup.second };
	//If we can form another group, do it  
	if (static_cast<int>(notPaired.size()) > 1) {
		//Go through each of the new subsets
		for (auto pairup : getSubsets(notPaired)) {
			//And initiate recursion
			listofpairedgroupings newgroupings{ getGroupings(pairup) };
			//For each new grouping that is available
			for (pairedgrouping newgroup : newgroupings) {
				//Create an empty container
				grouping toadd{};
				//Reserve the space
				toadd.reserve(currentgroups.size() + newgroup.first.size());
				//Insert the current grouping and the new grouping
				toadd.insert(toadd.end(), currentgroups.begin(), currentgroups.end());
				toadd.insert(toadd.end(), newgroup.first.begin(), newgroup.first.end());
				//Add this new grouping to the output
				list.push_back(std::make_pair(toadd, newgroup.second));
			}
		}
	}
	//AIM: Remove duplicates
	//Set up container for duplicates
	listofpairedgroupings nodupes{};
	//Loop over groupings
	for (int i{}; i < static_cast<int>(list.size()); ++i) {
		//Loop over each group
		for (int j{}; j < static_cast<int>(list[i].first.size()); ++j) {
			//Sort this group
			std::sort(list[i].first[j].begin(), list[i].first[j].end());
		}
		//Sort the overall grouping
		std::sort(list[i].first.begin(), list[i].first.end());
		//Check for duplicates
		bool isfound{ false };
		//Loop over the groupings we've already found
		for (auto dupe : nodupes) {
			if (dupe.first == list[i].first)
				isfound = true;
		}
		//If we haven't found this yet, add it to the list
		if (!isfound)
			nodupes.push_back(list[i]);
	}
	return nodupes;
}