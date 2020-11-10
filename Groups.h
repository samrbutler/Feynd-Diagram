#pragma once

#include "getSubsets.h"
#include "getSubsets.h"
#include "Model.h"
#include "Particle.h"

#include <algorithm>
#include <utility>
#include <vector>

listofpairedgroupings getGroupings(const pairedgrouping& pairup, const int min_size = 2);