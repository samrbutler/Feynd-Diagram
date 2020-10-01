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
    const std::multiset<std::multiset<P>> interactions{
        {P::phi,P::psi,P::antipsi},
    };

    intdict nto1s{ generateDictionary(interactions) };
    std::cout << nto1s.size() << '\n';



}