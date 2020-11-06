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

	Particle p1{ P::phi };
	Particle p2{ P::phi };

	Timer t;

	std::vector<Particle> x{ p1,p2 };

	auto returns{ connect1PI(x,2) };
	auto time{ t.elapsed() };

	for (LoopDiagram& ld : returns) {
		//std::cout << ld << '\n';
	}

	std::cout << "\n\nCompleted in " << time << " seconds.\n\n";
}