#pragma once

#include "Diagram.h"
#include "Interactions.h"
#include "LoopDiagram.h"
#include "LoopSignatures.h"
#include "LoopyVertex.h"
#include "Model.h"
#include "Particle.h"
#include "Subsets.h"
#include "Vertex.h"

#include <set>
#include <vector>

std::vector<LoopDiagram> connect1PI(const LoopDiagram& diag, const int num_loops, const std::vector<LoopyVertex>& loopyvs = Model::loopy_vertices,
	const n0dict& nto0 = Model::n_to_0, const n1dict& nto1 = Model::n_to_1);

std::vector<LoopDiagram> connect1PISubdiagram(LoopDiagram& diag, const int num_loops, const std::vector<LoopyVertex>& loopyvs = Model::loopy_vertices,
	const n0dict& nto0 = Model::n_to_0, const n1dict& nto1 = Model::n_to_1);

std::vector<LoopDiagram> connect1PIZero(LoopDiagram& diag, const n0dict& nto0 = Model::n_to_0, const n1dict& nto1 = Model::n_to_1);