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

	P p1{ P::psi };
	P p2{ P::antipsi };
	P p3{ P::psi };
	P p4{ P::antipsi };
		
	LoopDiagram p({ p1,p2, p3,p4 });
	Timer t;
	auto returns{ connect1PI(p,1,Model::loopy_vertices,Model::n_to_0,Model::n_to_1,false) };
	auto time{ t.elapsed() };
	for (Diagram& ld : returns) {
		std::cout << ld << '\n';
	}

	std::cout << "\n\nCompleted in " << time << " seconds.\n\n";
}