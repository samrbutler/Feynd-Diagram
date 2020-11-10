#pragma once

#include "LoopDiagram.h"
#include "LoopyVertex.h"
#include "Particle.h"

#include <vector>

std::vector<int> getLoopSignature(const std::vector<Particle>& inbound);

std::vector<int> invertLoopSignature(const std::vector<int>& sig);

/*newloopprofiles: List of pairs containing:
*	std::multiset<P> - the new outbound particle types
*	LoopyVertex   - pointer to the LoopyVertex responsible (or nullptr if not)
*/
using newloopprofiles = std::vector<std::pair<std::multiset<P>, LoopyVertex>>;

using newloopvalues = std::vector<std::pair<std::vector<Particle>, LoopyVertex>>;

newloopvalues spawnLoops(const std::vector<Particle>& inbound, const int max_loops, const std::vector<LoopyVertex>& loopyvs = Model::loopy_vertices);