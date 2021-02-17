#pragma once

#include "Diagram.h"
#include "Interactions.h"
#include "LoopyVertex.h"
#include "Model.h"
#include "Particle.h"
#include "Vertex.h"

#include <set>
#include <vector>

class LoopDiagram : public Diagram
{
public:

	static int global_loop_counter;

	LoopDiagram(const Diagram& diag) : Diagram(diag) {}

	LoopDiagram(const std::vector<Particle>& externs) : Diagram(externs) {}

	LoopDiagram(const std::vector<Particle>& externs, const Vertex& vertex) : Diagram(externs, vertex) {}

	//Override function connect to include connection with loops

	void addLoopyVertex(const LoopyVertex& lv, const std::vector<Particle>& inbound_parts);
};