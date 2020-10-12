/*FeyndDiagram.cpp
*	Function definitions
*		- main
*/

#include "FeyndDiagram.h"

#include "Diagram.h"
#include "Groups.h"
#include "Interactions.h"
#include "Model.h"
#include "Particles.h"
#include "Timer.h"

#include <iostream>

int main()
{
	
	/*TEST CODE
	Timer t;

	//Set up a scattering process
	Process proc({ P::psi, P::antipsi }, { P::psi, P::antipsi });

	//Generate the diagrams, specify interactiosn and debug output explicitly
	std::vector<Diagram> diagrams{ connect(proc, Model::Interactions, Model::NTO1, false) };

	//Print all the diagrams
	std::cout << "Found " << diagrams.size() << " diagrams in " << t.elapsed() << " seconds:\n\n";

	for (size_t i{}; i < diagrams.size(); ++i) {
		std::cout << "Diagram " << (i + 1) << ":\n" << diagrams[i];
	}
	*/
}