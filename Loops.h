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
	std::multiset<P> internal_particles;
	std::multiset<std::multiset<P>> internal_connections;
	int num_loops;
};

class LoopDiagram : public Diagram {

public:
	LoopDiagram(const std::vector<Particle>& externs) : Diagram(externs) {}

	LoopDiagram(const std::vector<Particle>& externs, const Vertex& vertex) : Diagram(externs, vertex) {}

	void addLoopyVertex(const LoopyVertex& lv, const std::vector<Particle> inbound_parts);
};

bool operator==(const LoopyVertex& lv1, const LoopyVertex& lv2);

std::vector<LoopyVertex> getLoopyVertices(const int max_loops, const loopdict& dictionary = Model::n_to_many);

namespace Model {
	inline const std::vector<LoopyVertex> loopy_vertices{ getLoopyVertices(Model::max_legs) };
}

std::vector<LoopDiagram> connectLoop1PI(const Diagram& diag, const int num_loops, const std::vector<LoopyVertex>& loopyvs = Model::loopy_vertices);