// Feynder.cpp : Define the 'main' function.
#include "Feynder.h"

#include "Diagram.h"
#include "Groups.h"
#include "Interactions.h"
#include "Particles.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <set>

int main()
{
    const n0dict interactions{
        {P::phi,P::psi,P::antipsi},
        {P::chi,P::psi,P::antipsi}
    };

    n1dict dictionary{ generateDictionary(interactions) };
}