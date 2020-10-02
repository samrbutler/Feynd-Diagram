// Interactions.cpp : introduce the code relevant to particle interactions
#include "Interactions.h"

#include "Particles.h"

#include <iostream>
#include <iterator>
#include <typeinfo>
#include <map>
#include <set>

//Given a list of allowable interaction vertices, produce a list of all allowable n-to-1 interactions
n1dict generateDictionary(const n0dict &inters) {

    //Create empty container
    n1dict nto1s{};

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
            std::pair<std::multiset<P>, P> toInsert {std::make_pair(lefthand, righthand)};
            //Add to the dictionary
            nto1s.insert(toInsert);
        }
    }
    //Return the populated dictionary
    return nto1s;

}

//Connect points to the vertex, return true if successful
bool Vertex::addLeg(std::vector<Point>& pointstoadd) {
    //Check if we can actually add this many legs
    if (m_numlegs- static_cast<int>(m_connections.size())- static_cast<int>(pointstoadd.size())<0) {
        return false;
    }
    //If we can, then do it
    else {
        for (auto point: pointstoadd)
            m_connections.push_back(point);
        return true;
    }
}

//
std::vector<P> getProducts(n1dict& dictionary, std::vector<Particle>& group) {
    std::vector<P> vec{};
    for (auto interaction : dictionary) {
        auto interactionleft = interaction.first;
        std::multiset<P> groupnames{};
        int activecount{};
        for (auto element : group) {
            activecount += element.isActive();
            groupnames.insert(element.getType());
        }
        if ((groupnames == interactionleft)&&(activecount>=1)) {
            vec.push_back(interaction.second);
        }
    }
    return vec;
}