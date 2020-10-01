// Feynder.cpp : Define the 'main' function.

#include <set>
#include <map>
#include <iterator>
#include <typeinfo>
#include <iostream>
#include <algorithm>

#include "Particles.h"
#include "Interactions.h"

int main()
{
    const n0dict interactions{
        {P::phi,P::psi,P::antipsi},
    };

    n1dict nto1{ generateDictionary(interactions) };

}