/*Feynder.cpp
    Function definitions
        - int main()
*/

#include "Feynder.h"

#include "Diagram.h"
#include "Groups.h"
#include "Interactions.h"
#include "Model.h"
#include "Particles.h"

#include <iostream>

int main()
{
    //TEST CODE

    //Set up a scattering process
    Process proc({ P::psi,P::antipsi }, { P::psi,P::antipsi });

    //Generate the diagrams, specify interactiosn and debug output explicitly
    auto diagrams{ connect(proc, Model::Interactions, Model::NTO1, true) };

    //Print all the diagrams
    for (int i{}; i < static_cast<int>(diagrams.size()); ++i) {
        std::cout << "Diagram "<< (i+1) << ":\n" << diagrams[i];
    }
}