#pragma once

#include "Diagram.h"
#include "Groups.h"
#include "Model.h"
#include "Particle.h"

#include <numeric>
#include <set>

struct LoopyVertex
{
	std::multiset<P> external_particles_in;
	std::multiset<P> external_particles_out;
	std::multiset<P> internal_particles;
	std::multiset<std::multiset<P>> internal_connections;
	int num_loops;
	bool is_null{ false };
};

static inline const LoopyVertex LV_null{ {}, {}, {}, {}, 0, true };

bool operator==(const LoopyVertex& lv1, const LoopyVertex& lv2);

std::vector<LoopyVertex> getLoopyVertices(const int max_loops, const loopdict& dictionary = Model::n_to_many);

namespace Model
{
	inline const std::vector<LoopyVertex> loopy_vertices{ getLoopyVertices(Model::max_legs) };
}