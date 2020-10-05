/*Model.cpp :
    Function definitions
        - P getAntiParticle(const P part)
            > Given a particle, return the name of its antiparticle
        - n1dict generateDictionary(const n0dict& inters)
            > Given a list of allowable interaction vertices, produce a list of all allowable n-to-1 interactions
*/

#include "Model.h"

#include <iterator>
#include <map>
#include <set>
#include <tuple>
#include <vector>

//Given a particle, return the name of its antiparticle
P getAntiParticle(const P part) { return Model::AntiParticleDict.at(part); }

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