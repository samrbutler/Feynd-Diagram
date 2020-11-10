#pragma once

#include "Diagram.h"
#include "Groups.h"
#include "Interactions.h"
#include "Model.h"
#include "Particle.h"
#include "Vertex.h"

#include <utility>
#include <vector>

std::vector<Diagram> connect(Diagram& diag, const n0dict& nto0 = Model::n_to_0, const n1dict& nto1 = Model::n_to_1, const bool debug = false);