/*Model.cpp :
* 
*/

#include "Model.h"

#include <algorithm>
#include <iterator>
#include <iostream>
#include <map>
#include <string>
#include <set>
#include <tuple>
#include <vector>

//Allow for output of ParticleTypes to std::cout
std::ostream& operator<< (std::ostream& out, const P part) {
	//Switch based on the particle type
	switch (part) {
	case P::phi:
		out << "Phi";
		break;
	case P::chi:
		out << "Chi";
		break;
	case P::psi:
		out << "Psi";
		break;
	case P::antipsi:
		out << "Antipsi";
		break;
	}
	return out;
}

//Given a particle, return the name of its antiparticle
P getAntiParticle(const P part) { return Model::AntiParticleDict.at(part); }

//Given a list of allowable interaction vertices, produce a list of all allowable n-to-1 interactions
n1dict generateN1Dictionary(const n0dict& inters) {

	//Create empty container
	n1dict nto1s{};

	//Loop over all the interactions via iterators
	for (n0dict::const_iterator interaction{ inters.begin() }; interaction != inters.end(); ++interaction) {

		//Aim: Identify each element in turn and make it the product of an n-to-1 interaction

		//Loop over all the particles in the current interaction
		for (size_t i{}; i < interaction->size(); ++i) {

			//Prepare a copy of the multiset...
			std::multiset<P> lefthand = *interaction;

			//...and store its location
			std::multiset<P>::iterator blh{ lefthand.begin() };

			//Advance the pointer to the correct particle
			std::advance(blh, i);

			//Extract the product particle and store
			P righthand = *blh;

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

//Given the n->1 dictionary, return all possible k->m interactions where m>2
loopdict getLoopDictionary(const n1dict& inters) {

	//Create empty container
	loopdict dict;

	//Loop over all the interactions
	for (const std::pair<std::multiset<P>, P>& interaction : inters) {

		size_t s{ interaction.first.size() };

		//Loop through how many incoming legs we're going to have
		for (int num_incoming{ 1 }; num_incoming < static_cast<int>(s); ++num_incoming) {
			//We need to select num_incoming-1 particles to move from the left hand of the interaction to the right
			//Set up a bitmask with num_incoming-1 1s and then enough 0s to make up the slack
			std::string bitmask(num_incoming - 1, '1');
			bitmask.resize(s, '0');
			do {
				//Keep the current output particle as a new input particle
				std::multiset<P> newleft{ getAntiParticle(interaction.second) };
				std::multiset<P> newright;
				for (size_t i{}; i < s; ++i)
				{
					//Get a pointer to the first element of the current left-hand-side of the interaction
					std::multiset<P>::iterator start{ interaction.first.begin() };
					//Advance the pointer to the current particle
					std::advance(start, i);
					//Add the element to newleft if the bit is 1
					if (bitmask[i] == '1') {
						newleft.insert(getAntiParticle(*start));
					}
					else {
						newright.insert(*start);
					}
				}
				//Add this new interaction to the dictionary
				dict.insert(std::make_pair(newleft, newright));
				//Then go to the previous permutation
			} while (std::prev_permutation(bitmask.begin(), bitmask.end()));
		}
	}
	return dict;
}