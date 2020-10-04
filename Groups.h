/*Groups.h :
	Type aliases
		- grouping
			> a list of groups of particles
		- pairedgrouping
			> a pair consisting of a grouping and a list of ungrouped particles
		- listofgroupings
		- listofpairedgroupings
	Function declarations
		- operator==
		- getSubsets
		- getGroupings
*/

#pragma once

#include "Particles.h"

#include <algorithm>
#include <functional>
#include <utility>
#include <vector>

//List of groups of particles
using grouping = std::vector<std::vector<Particle>>;

//Pair consisting of a grouping and a list of ungrouped particles
using pairedgrouping = std::pair<grouping, std::vector<Particle>>;

using listofgroupings = std::vector<grouping>;
using listofpairedgroupings = std::vector<pairedgrouping>;

bool operator==(const grouping& part1, const grouping& part2);

listofpairedgroupings getSubsets(std::vector<Particle>& input);
listofpairedgroupings getGroupings(pairedgrouping& pairup);