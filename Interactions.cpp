/*Interactions.cpp :
    Vertex class: member function definitions
        - bool Vertex::addLegs(const std::vector<int>& idstoadd, const std::vector<P>& typestoadd)
            > Add a point to the vertex if we can and return true if this was successful
    Function definitions
        - std::multiset<P> vec2multiset(const std::vector<Particle>& group)
            > Given a vector of particles, return a multiset of particlenames
        - n1dict generateDictionary(const n0dict& inters)
            > Given a list of allowable interaction vertices, produce a list of all allowable n-to-1 interactions
        - bool isGroupingValid(pairedgrouping& pair, const n1dict& dictionary)
            > Given a 'pairedgrouping' and a n->1 interaction dictionary, return if the pairedgrouping produces allowable interactions
        - std::vector<P> getProducts(std::vector<Particle>& group, const n1dict& dictionary)
            > Given a particular particle group, return the antiparticles of products of the interaction from a dictionary
            > Antiparticles are returned ready for use as external vertices in the next subdiagram
        - listofproducts getNewExterns(pairedgrouping& pair,const n1dict& nto1)
            > Given a 'pairedgrouping' and a n->1 interaction dictionary, return the products expected from the implied interaction
*/

#include "Interactions.h"

#include "Groups.h"
#include "Particles.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <set>

//Add a point to the vertex if we can and return true if this was successful
bool Vertex::addLegs(const std::vector<int>& idstoadd,const std::vector<P>& typestoadd) {
    //Return early if the number of legs will exceed capacity
    if (m_numlegs- static_cast<int>(m_connection_ids.size())- static_cast<int>(idstoadd.size())<0) return false;
    else {
        //Go through and add all ids and types to the vertex
        for (int i{}; i < static_cast<int>(m_connection_ids.size()); ++i) {
            m_connection_ids.push_back(idstoadd[i]);
            m_connection_types.push_back(typestoadd[i]);
        }

        //Success
        return true;
    }
}

//Given a vector of particles, return a multiset of particlenames
std::multiset<P> vec2multiset(const std::vector<Particle>& group) {
    //Create an empty multiset
    std::multiset<P> groupnames{};

    //Add the type of every element in the group to the multiset
    for (auto element : group) {
        groupnames.insert(element.getType());
    }

    return groupnames;
}

//Given a list of allowable interaction vertices, produce a list of all allowable n-to-1 interactions
n1dict generateDictionary(const n0dict& inters) {

    //Create empty container
    n1dict nto1s{};

    //Loop over all the interactions via iterators
    for (auto interaction{ inters.begin() }; interaction != inters.end(); ++interaction) {

        //Aim: Identify each element in turn and make it the product of an n-to-1 interaction

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
            std::pair<std::multiset<P>, P> toInsert{ std::make_pair(lefthand, righthand) };

            //Add to the dictionary
            nto1s.insert(toInsert);
        }
    }

    //Return the populated dictionary
    return nto1s;
}

//Given a 'pairedgrouping' and a n->1 interaction dictionary, return if the pairedgrouping produces allowable interactions
bool isGroupingValid(pairedgrouping& pair, const n1dict& dictionary) {
    //Get the grouplist
    grouping grouplist{ pair.first };

    //Exit early if the group is empty
    if (static_cast<int>(grouplist.size()) == 0) return false;

    //Loop over all groups in the grouplist: if any group is invalid the whole grouping is invalid
    for (auto group : grouplist) {
        //Count the number of active particles: if this is less than 1, the group isn't valid
        if (std::count_if(group.begin(), group.end(), [](Particle& p) -> bool {return p.isActive(); }) < 1) return false;

        //Get a multiset of particle names in the current group
        auto groupnames{ vec2multiset(group) };

        //Suppose the group isn't valid
        bool validgroup{ false };

        //Loop over all interactions in the dictionary
        for (auto interaction : dictionary) {
            //If the current group matches this interaction, the group is valid
            if (groupnames == interaction.first) {
                validgroup = true;
            }
        }
        if (!validgroup) return false;
    }

    //If all groups are valid, the grouping is valid
    return true;
}

//Given a particular particle group, return the antiparticles of products of the interaction from a dictionary
std::vector<P> getProducts(std::vector<Particle>& group, const n1dict& dictionary) {
    //Empty return vector
    std::vector<P> vec;

    //Return an empty vector early if the number of active particles is less than 1
    if (std::count_if(group.begin(), group.end(), [](Particle& p) -> bool {return p.isActive(); }) < 1) return vec;

    //Get the names of particles in the group
    auto groupnames{vec2multiset(group)};

    //Loop over all interactions in the dictionary
    for (auto interaction : dictionary) {
        //Check if the multiset matches the current interaction
        if (groupnames == interaction.first) {
            //If it does, add its antiparticle to the product list
            vec.push_back(getAntiParticle(interaction.second));
        }
    }

    return vec;
}

//Given a 'pairedgrouping' and a n->1 interaction dictionary, return the products expected from the implied interaction
listofproducts getNewExterns(pairedgrouping& pair,const n1dict& nto1) {
    //Empty container
    listofproducts container;

    //If this grouping is invalid, add an empty vector
    if (!isGroupingValid(pair, nto1)) {
        container.push_back({});
    }
    else {
        //Get the grouped elements
        auto groups{ pair.first };

        //Count how many groups there are
        int numgroups = static_cast<int>(groups.size());

        //Receive a list of possible products for each group
        std::vector<std::vector<P>> productlist;
        for (auto group : groups) {
            productlist.push_back(getProducts(group, nto1));
        }

        //Aim : Produce all possible combinations of selecting one element for each group

        //Set up index counters for each of the groups
        int* indexmax{ new int[numgroups] {} };
        int* currentindex{ new int[numgroups] {} };
        for (int i{}; i < numgroups; ++i) {
            currentindex[i] = 0;
            indexmax[i] = static_cast<int>(productlist[i].size());
        }

        while (true) {
            //Set up container
            std::vector<P> toadd{};

            //Add the current combination implied by the index array
            for (int i{}; i < numgroups; ++i) {
                toadd.push_back(productlist[i][currentindex[i]]);
            }
            container.push_back(toadd);

            //Go to the last group
            int nextgroup{ numgroups - 1 };

            //Find the next group that still has elements left to select
            while (nextgroup >= 0 && (currentindex[nextgroup] + 1 >= indexmax[nextgroup])) {
                --nextgroup;
            }

            //If it wasn't found, quit now
            if (nextgroup < 0) break;

            //If it was, increment the index
            ++currentindex[nextgroup];

            //Then set all indices for later groups back to 0 to loop again
            for (int i{ nextgroup + 1 }; i < numgroups; ++i) {
                currentindex[i] = 0;
            }
        }
        //Clean up
        delete[] currentindex;
        delete[] indexmax;
    }
    //Return the list
    return container;
}