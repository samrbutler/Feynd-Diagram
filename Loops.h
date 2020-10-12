/*Loops.h :
*
*	Function declarations
*/

#pragma once

#include "Diagram.h"
#include "Model.h"
#include "Interactions.h"

std::vector<Diagram> getLoopTopologies(const std::vector<Particle>& legs, const int num_loops);