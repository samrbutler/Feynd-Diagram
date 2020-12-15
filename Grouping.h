#pragma once

#include "Model.h"
#include "numBitsSet.h"
#include "Particle.h"
#include "Subsets.h"

#include <algorithm>
#include <limits>
#include <set>
#include <utility>
#include <vector>

bool operator==(const group& part1, const group& part2);

class Grouping
{
public:
	listofpairedgroups possible_groupings;

	listofpairedgroups getGroupings(const pairedgroup& pairup, const int min_size);
public:
	Grouping(const std::vector<Particle>& particles, const int min_size = 2);
};