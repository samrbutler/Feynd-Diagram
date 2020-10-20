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

	Particle p1{ P::phi };
	Particle p2{ P::phi };

	LoopDiagram diag(std::vector<Particle> {p1, p2});

	auto returns{ connectLoop1PI(diag,1) };
	for (LoopDiagram& ld : returns) {
		std::cout << ld;
	}

	std::cout << "\n\nCompleted in " << t.elapsed() << " seconds.\n\n";
}