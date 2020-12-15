#pragma once

#include "Particle.h"

#include <vector>

using grouping = std::vector<std::vector<Particle>>;

using pairedgrouping = std::pair<grouping, std::vector<Particle>>;

using listofgroupings = std::vector<grouping>;

using listofpairedgroupings = std::vector<pairedgrouping>;

//Deprecated: pass to new function
listofpairedgroupings getSubsets(const std::vector<Particle>& input, const size_t min_size = 2);
//Deprecated: pass to new function
listofpairedgroupings getSubsets(const std::vector<Particle>& input, const size_t min_size, const size_t max_size);