#include "Groups.h"

//Given a partially completed 'pairedgrouping', return all possible completions of the grouping without duplicates
listofpairedgroupings getGroupings(const pairedgrouping& pairup, const int min_size)
{
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