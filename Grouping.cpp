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
	return list;
}

Grouping::Grouping(const std::vector<Particle>& particles, const int min_size)
{
	listofpairedgroups list{ getGroupings(std::make_pair(group{}, particles), min_size) };

	//Now remove duplicates
	
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

	possible_groupings = nodupes;

}
