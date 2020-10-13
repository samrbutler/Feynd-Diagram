/*Groups.h :
*
*/

#pragma once

#include "Model.h"
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

listofpairedgroupings getSubsets(const std::vector<Particle>& input, const size_t min_size = 2);
listofpairedgroupings getGroupings(const pairedgrouping& pairup);