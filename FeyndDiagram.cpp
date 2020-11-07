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

	Timer t;

	std::vector<Particle> x{ p1,p2,p3,p4 };
	Process p({ p1,p2 }, { p3,p4 });
	auto returns{ connect(p) };
	auto time{ t.elapsed() };

	for (Diagram& ld : returns) {
		std::cout << ld << '\n';
	}

	std::cout << "\n\nCompleted in " << time << " seconds.\n\n";
}