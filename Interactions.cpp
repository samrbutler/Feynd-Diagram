// Interactions.cpp : introduce the code relevant to particle interactions

#include <set>
#include <map>
#include <iterator>
#include <iostream>

#include "Interactions.h"
#include "Particles.h"

//Given a list of allowable interaction vertices, produce a list of all allowable n-to-1 interactions
intdict generateDictionary(const std::multiset<std::multiset<P>> &inters) {

    //Create empty container
    intdict nto1s{};

    //Loop over all the interactions via iterators
    for (auto interaction{ inters.begin() }; interaction != inters.end(); ++interaction) {
        
        //AIM: Identify each element in turn and make it the product of an n-to-1 interaction
            
        //Loop over all the particles in the current interaction
        for (int i{}; i < static_cast<int>(interaction->size()); ++i) {

            //Prepare a copy of the multiset...
            auto lefthand = *interaction;
            //...and store its location
            auto blh{ lefthand.begin() };
            //Advance the pointer to the correct particle
            std::advance(blh, i);
            //Extract the product particle and store
            auto righthand = *blh;
            //Remove this particle from the left hand side
            lefthand.erase(blh);
            //Pair up the left and right hand sides
            std::multimap<std::multiset<P>, P> toInsert {std::make_pair(lefthand, righthand)};
            //Add to the dictionary
            nto1s.insert(toInsert);
        }
    }
    //Return the populated dictionary
    return nto1s;

}

bool Vertex::addLeg(Point pointtoadd) {
    if (static_cast<int>(m_connections.size()) >= m_numlegs) {
        return false;
    }
    else {
        m_connections.push_back(pointtoadd);
        return true;
    }
}