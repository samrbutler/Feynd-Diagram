/*Loops.h :
*
*/

#pragma once

#include "Diagram.h"
#include "Model.h"
#include "Interactions.h"

#include <set>

struct LoopyVertex {
	std::multiset<P> external_particles_in;
	std::multiset<P> external_particles_out;
	std::multiset<std::multiset<P>> internal_connections;
	int num_loops;
};

bool operator==(const LoopyVertex& lv1, const LoopyVertex& lv2);

std::vector<LoopyVertex> getLoopyVertices(const int max_loops, const loopdict& dictionary = Model::n_to_many);

std::vector<Diagram> getLoopTopologies(const std::vector<Particle>& legs, const int num_loops);