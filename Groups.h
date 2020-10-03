// Groups.h : set up for groupings

#pragma once

#include "Particles.h"

#include <algorithm>
#include <functional>
#include <utility>
#include <vector>

//Define the type aliases for groupings
using grouping = std::vector<std::vector<Particle>>;
using pairedgrouping = std::pair<grouping, std::vector<Particle>>;
using listofgroupings = std::vector<grouping>;
using listofpairedgroupings = std::vector<pairedgrouping>;

listofpairedgroupings getSubsets(std::vector<Particle>& input);

bool operator<(const std::vector<Particle>& vec1, const std::vector<Particle>& vec2);
bool operator!=(const std::vector<Particle>& part1, const std::vector<Particle>& part2);
bool operator==(const grouping& part1, const grouping& part2);

listofpairedgroupings getGroupings(pairedgrouping& pairup);