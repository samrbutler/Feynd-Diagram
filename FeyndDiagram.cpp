/*FeyndDiagram.cpp
* 
*/

#include "FeyndDiagram.h"

#include "Diagram.h"
#include "Groups.h"
#include "Interactions.h"
#include "Loops.h"
#include "Model.h"
#include "Particles.h"
#include "Timer.h"

#include <iostream>
#include <set>

int main()
{
	
	//TEST CODE
	Timer t;

	for (const LoopyVertex& lv : Model::loopy_vertices) {
		auto extern_in = lv.external_particles_in;
		auto extern_out = lv.external_particles_out;
		auto interc = lv.internal_connections;
		std::cout << "{ ";
		for (const P& el : extern_in) {
			std::cout << el << ' ';
		}
		std::cout << ", ";
		for (const P& el : extern_out) {
			std::cout << el << ' ';
		}
		std::cout << "} , { ";
		for (const std::multiset<P>& el : interc) {
			std::cout << "[ ";
			for (const P& el2 : el) {
				std::cout << el2 << ' ';
			}
			std::cout << "] ";
		}
		std::cout << "}\n";
	}

	std::cout << "\n\nCompleted in " << t.elapsed() << " seconds.\n\n";
}