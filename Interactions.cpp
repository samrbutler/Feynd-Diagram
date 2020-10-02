// Interactions.cpp : introduce the code relevant to particle interactions
#include "Interactions.h"

#include "Particles.h"

#include <algorithm>
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

std::multiset<P> vec2multiset(std::vector<Particle>& group) {
    std::multiset<P> groupnames{};
    for (auto element : group) {
        //Transfer the particle name to the multiset
        groupnames.insert(element.getType());
    }
    return groupnames;
}

//Given a particular particle group, return the antiparticles of products of the interaction from a dictionary
//Note: antiparticles are returned ready for use as external vertices in the next subdiagram
std::vector<P> getProducts(n1dict& dictionary, std::vector<Particle>& group) {
    //Empty return vector
    std::vector<P> vec{};
    //Count the number of active Particles
    int activecount{ std::count_if(group.begin(),group.end(),[](Particle& p) -> bool {return p.isActive(); }) };
    //Return early if this is less than 1
    if (activecount < 1)
        return vec;
    auto groupnames{vec2multiset(group)};
    //Loop over all interactions in the dictionary
    for (auto interaction : dictionary) {
        //Check if the multiset matches the current interaction
        if (groupnames == interaction.first) {
            //And add its antiparticle to the product list
            vec.push_back(getAntiParticle(interaction.second));
        }
    }
    return vec;
}