#include "Process.h"

//Create a process from incoming and outgoing particle names
Process::Process(const std::vector<P> incoming, const std::vector<P> outgoing)
{
	for (P part : incoming) {
		//Construct an active particle and add it to the list
		m_externs.push_back(Particle(part, true));
	}
	for (P part : outgoing) {
		//Construct an active antiparticle and add it to the list
		m_externs.push_back(Particle(getAntiParticle(part), true));
	}
	//Store the particle names
	m_incoming = incoming;
	m_outgoing = outgoing;
}