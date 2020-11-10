#include "LoopDiagram.h"

//Initialise global loop counter in LoopDiagram
int LoopDiagram::global_loop_counter{ 1 };

void LoopDiagram::addLoopyVertex(const LoopyVertex& lv, const std::vector<Particle>& inbound_parts)
{
	//Generate the internal particles
	std::vector<Particle> intparticles;
	std::vector<Particle> allparticles{ inbound_parts };
	for (const P& intpart : lv.internal_particles) {
		intparticles.push_back(Particle(intpart));
	}
	allparticles.insert(allparticles.end(), intparticles.begin(), intparticles.end());
	//Generate and add the central vertex
	this->addVertex(Vertex(allparticles));
	//Generate and add the internal vertices
	for (const std::multiset<P>& group : lv.internal_connections) {
		std::vector<Particle> toadd;
		for (const P& ptype : group) {
			for (size_t i{}; i < intparticles.size(); ++i) {
				if (intparticles[i].getType() == ptype) {
					toadd.push_back(intparticles[i]);
					intparticles.erase(intparticles.begin() + i);
					break;
				}
			}
		}
		this->addVertex(Vertex(toadd));
	}
}