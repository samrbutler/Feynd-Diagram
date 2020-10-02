// Diagram.cpp : Code relevant to diagram construction and manipulation

#include <vector>
#include <utility>
#include <bits.h>
#include "Diagram.h"
#include "Particles.h"
#include "Interactions.h"

//Defines function groupify to take interaction dictionariesand a vector of Particles, returning all possible allowed groupings
void Diagram::groupify(const n0dict& nto0, const n1dict& nto1) {
	std::vector<Particle> nodes = m_externs;
	

}

grouping getSubsets(std::vector<Particle> input) {
	std::vector<std::pair<std::vector<std::vector<Particle>>, std::vector<Particle>>> pairings{};
	for (int i{}; i < pow(2, input.size()); ++i) {
		std::vector<Particle> subset{};
		std::vector<Particle> notsubset{};
		for (int j{}; j < static_cast<int>(input.size()); ++j) {
			if ((i & (1 << j)) != 0)
				subset.push_back(input[j]);
			else
				notsubset.push_back(input[j]);
		}
		if (subset.size() >= 2) {
			std::vector<std::vector<Particle>> toadd{};
			toadd.push_back(subset);
			auto pairup{ std::make_pair(toadd, notsubset) };
			pairings.push_back(pairup);
		}
		
	}
	return pairings;
}

std::vector<std::vector<std::vector<Particle>>> getGroupings(std::pair<std::vector<std::vector<Particle>>, std::vector<Particle>> pairup) {
	//Create the empty vector of groupings
	std::vector<std::vector<std::vector<Particle>>> listofgroups{};
	//Extract the current vector of groups...
	auto currentgroups{ pairup.first };
	//...and add it to the new vector of groupings (i.e. we terminate pairing here)
	listofgroups.push_back(currentgroups);
	//Get the vector of ungrouped elements
	auto notPaired{ pairup.second };
	//If we can form another group, do it  
	if (static_cast<int>(notPaired.size()) > 1) {
		//Go through each of the new subsets
		for (auto pairup : getSubsets(notPaired)) {
			//And initiate recursion
			auto newgroupings{ getGroupings(pairup) };
			//For each new grouping that is available
			for (auto newgroup : newgroupings) {
				//Create an empty container
				std::vector<std::vector<Particle>> toadd{};
				//Reserve the space
				toadd.reserve(currentgroups.size() + newgroup.size());
				//Insert the current grouping and the new grouping
				toadd.insert(toadd.end(), currentgroups.begin(), currentgroups.end());
				toadd.insert(toadd.end(), newgroup.begin(), newgroup.end());
				//Add this new grouping to the output
				listofgroups.push_back(toadd);
			}
		}		
	}
	return listofgroups;
}
