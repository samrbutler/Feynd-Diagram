#pragma once

#include "Model.h"
#include "numBitsSet.h"
#include "Particle.h"

#include <algorithm>
#include <limits>
#include <set>
#include <utility>
#include <vector>

//A pair, where the first element is a vector of included Particles and the second element is a vector of excluded Particles
using pairedsubset = std::pair<std::vector<Particle>, std::vector<Particle>>;
//A vector of pairs, where the first element of each pair is a vector of included Particles and the second element of each pair is a vector of excluded Particles
using listofpairedsubsets = std::vector<pairedsubset>;
//A vector of particle collections, each of which is a vector of Particles
using group = std::vector<std::vector<Particle>>;
//A pair, where the first element of each pair is a vector of particle collections, each of which are vectors of Particles, and the second element is a vector of ungrouped Particles
using pairedgroup = std::pair<group, std::vector<Particle>>;
//A vector of groups, each of which is a vector of particle collections, each of which is a vector of Particles
using listofgroups = std::vector<group>;
//A vector of pairs, where the first element of each pair is a vector of particle collections, each of which are vectors of Particles, and the second element of each pair is a vector of ungrouped Particles
using listofpairedgroups = std::vector<pairedgroup>;

class SubsetList
{
public:
	listofpairedsubsets subsets;
	listofpairedgroups subsets_as_groups;

	SubsetList(const std::vector<Particle>& input, const size_t min_size,
		const size_t max_size = (size_t)std::numeric_limits<int>::max);
};