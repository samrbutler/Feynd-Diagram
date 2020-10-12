/*Loops.h :
*	Classes
*		- LoopVertex
*
*	Function declarations
*/

#pragma once

#include "Diagram.h"
#include "Model.h"
#include "Interactions.h"

class LoopVertex {

	std::vector<P> m_externs;
	std::vector<Vertex> m_interns;

public:

	const std::vector<P>& getExternalTypes() const { return m_externs; }
	const std::vector<Vertex>& getInternalVertices() const { return m_interns; }

	LoopVertex() = default;

	LoopVertex(const std::vector<P>& externs, const std::vector<Vertex>& internals)
		: m_externs{ externs }, m_interns{ internals } {}
};

std::vector<LoopVertex> getLoopTopologies(const std::vector<Particle>& legs, const int num_loops);