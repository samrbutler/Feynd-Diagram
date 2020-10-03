// Interactions.cpp : introduce the code relevant to particle interactions
#include "Interactions.h"

#include "Groups.h"
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
bool Vertex::addLegs(std::vector<int>& pointstoadd) {
    //Check if we can actually add this many legs
    if (m_numlegs- static_cast<int>(m_connection_ids.size())- static_cast<int>(pointstoadd.size())<0) {
        return false;
    }
    //If we can, then do it
    else {
        for (auto point: pointstoadd)
            m_connection_ids.push_back(point);
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

bool isGroupingValid(pairedgrouping& pair, const n1dict& dictionary) {
    //Get the grouplist
    grouping grouplist{ pair.first };
    if (static_cast<int>(grouplist.size()) == 0) return false;
    //Loop over all groups in the grouplist: if any group is invalid the whole grouping is invalid
    for (auto group : grouplist) {
        //Count the number of active particles: if this is less than 1, the group isn't valid
        int activecount{ std::count_if(group.begin(),group.end(),[](Particle& p) -> bool {return p.isActive(); }) };
        if (activecount < 1) return false;
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
        if (!validgroup)
            return false;
    }
    //If all groups are valid, the grouping is valid
    return true;
}

//Given a particular particle group, return the antiparticles of products of the interaction from a dictionary
//Note: antiparticles are returned ready for use as external vertices in the next subdiagram
std::vector<P> getProducts(std::vector<Particle>& group, const n1dict& dictionary) {
    //Empty return vector
    std::vector<P> vec;
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

listofpairedproducts getNewExterns(listofpairedgroupings& grps,const n1dict& nto1) {
    listofpairedproducts container;
    //Loop through each grouping
    for (pairedgrouping pair : grps) {
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
            //Set up index counters for each of the groups
            int* indexmax{ new int[numgroups] {} };
            int* currentindex{ new int[numgroups] {} };
            for (int i{}; i < numgroups; ++i) {
                currentindex[i] = 0;
                indexmax[i] = static_cast<int>(productlist[i].size());
            }
            //Produce all possible combinations of selecting one element for each group
            while (true) {
                std::vector<P> toadd{};
                for (int i{}; i < numgroups; ++i) {
                    toadd.push_back(productlist[i][currentindex[i]]);
                }
                container.push_back(std::make_pair(toadd, pair.second));
                int nextgroup{ numgroups - 1 };
                while (nextgroup >= 0 && (currentindex[nextgroup] + 1 >= indexmax[nextgroup])) {
                    --nextgroup;
                }
                if (nextgroup < 0) break;
                currentindex[nextgroup]++;
                for (int i{ nextgroup + 1 }; i < numgroups; ++i) {
                    currentindex[i] = 0;
                }
            }
            delete[] currentindex;
            delete[] indexmax;
        }
    }
    return container;
}
