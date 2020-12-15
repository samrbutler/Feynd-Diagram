#pragma once

#include "Model.h"
#include "numBitsSet.h"
#include "Particle.h"

#include <algorithm>
#include <limits>
#include <set>
#include <utility>
#include <vector>

using pairedsubset = std::pair<std::vector<Particle>, std::vector<Particle>>;
using listofpairedsubsets = std::vector<pairedsubset>;

using group = std::vector<std::vector<Particle>>;
using pairedgroup = std::pair<group, std::vector<Particle>>;
using listofgroups = std::vector<group>;
using listofpairedgroups = std::vector<pairedgroup>;

class SubsetList
{
public:
	listofpairedsubsets subsets;
	listofpairedgroups subsets_as_groups;

	SubsetList(const std::vector<Particle>& input, const size_t min_size,
		const size_t max_size = (size_t)std::numeric_limits<int>::max);
};