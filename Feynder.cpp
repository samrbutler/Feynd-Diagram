/*Feynder.cpp
    Function definitions
        - int main()
*/

#include "Feynder.h"

#include "Diagram.h"
#include "Groups.h"
#include "Interactions.h"
#include "Particles.h"

#include <iostream>

int main()
{
    //TEST CODE

    //Define the n->0 dictionary as a Yukawa theory with two fermions and two scalars
    const n0dict interactions{
        {P::phi,P::psi,P::antipsi},
        {P::chi,P::psi,P::antipsi}

    };

    //Generate the n->1 dictionary
    n1dict dictionary{ generateDictionary(interactions) };

    //Set up a scattering process
    Process proc({ P::psi,P::antipsi }, { P::psi,P::antipsi });

    //Generate the diagrams, specify debug output explicitly
    auto diagrams{ connect(proc,interactions,dictionary,true) };

    //Print all the diagrams
    for (int i{}; i < static_cast<int>(diagrams.size()); ++i) {
        std::cout << "Diagram "<< (i+1) << ":\n" << diagrams[i];
    }
}