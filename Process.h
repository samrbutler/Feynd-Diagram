#pragma once

#include "Diagram.h"
#include "Model.h"
#include "Particle.h"

#include <vector>

//Extend the diagram class to allow initial setup of a process
class Process : public Diagram
{
	//Incoming particle names
	std::vector<P> m_incoming;
	//Outgoing particle names
	std::vector<P> m_outgoing;

public:

	//Create a process from incoming and outgoing particle names
	Process(const std::vector<P> incoming, const std::vector<P> outgoing);
};