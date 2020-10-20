/*Interactions.cpp : 
*/

#include "Interactions.h"

#include "Groups.h"
#include "Model.h"
#include "Particles.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <set>

Vertex::Vertex(const std::vector<Particle>& parts, const bool isProp) {
	m_numlegs = static_cast<int>(parts.size());
	m_vertextype = {};
	m_isProp = isProp;
	for (const Particle& part : parts) {
		m_connection_ids.push_back(part.getID());
		m_connection_types.push_back(part.getType());
	}
}

//Add a point to the vertex if we can and return true if this was successful
bool Vertex::addLegs(const std::vector<int>& idstoadd, const std::vector<P>& typestoadd) {
	//Return early if the number of legs will exceed capacity
	if (m_numlegs - m_connection_ids.size() - idstoadd.size() < 0) return false;
	else {
		//Go through and add all ids and types to the vertex
		for (size_t i{}; i < idstoadd.size(); ++i) {
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
	for (const Particle& element : group) {
		groupnames.insert(element.getType());
	}

	return groupnames;
}

//Given a 'pairedgrouping' and a n->1 interaction dictionary, return if the pairedgrouping produces allowable interactions
bool isGroupingValid(const pairedgrouping& pair, const n1dict& dictionary) {
	//Get the grouplist
	grouping grouplist{ pair.first };

	//Exit early if the group is empty
	if (grouplist.size() == 0) return false;

	//Loop over all groups in the grouplist: if any group is invalid the whole grouping is invalid
	for (const std::vector<Particle>& group : grouplist) {
		//Count the number of active particles: if this is less than 1, the group isn't valid
		if (std::count_if(group.begin(), group.end(), [](const Particle& p) -> bool {return p.isActive(); }) < 1) return false;

		//Get a multiset of particle names in the current group
		const std::multiset<P> groupnames{ vec2multiset(group) };

		//Suppose the group isn't valid
		bool validgroup{ false };

		//Loop over all interactions in the dictionary
		for (const std::pair<std::multiset<P>, P>& interaction : dictionary) {
			//If the current group matches this interaction, the group is valid
			if (groupnames == interaction.first) {
				validgroup = true;
				break;
			}
		}
		if (!validgroup) return false;
	}

	//If all groups are valid, the grouping is valid
	return true;
}

//Given a particular particle group, return the antiparticles of products of the interaction from a dictionary
std::vector<P> getProducts(const std::vector<Particle>& group, const n1dict& dictionary) {
	//Empty return vector
	std::vector<P> vec;

	//Return an empty vector early if the number of active particles is less than 1
	if (std::count_if(group.begin(), group.end(), [](const Particle& p) -> bool {return p.isActive(); }) < 1) return vec;

	//Get the names of particles in the group
	std::multiset<P> groupnames{ vec2multiset(group) };

	//Loop over all interactions in the dictionary
	for (const std::pair<std::multiset<P>, P>& interaction : dictionary) {
		//Check if the multiset matches the current interaction
		if (groupnames == interaction.first) {
			//If it does, add its antiparticle to the product list
			vec.push_back(getAntiParticle(interaction.second));
		}
	}

	return vec;
}

//Given a 'pairedgrouping' and a n->1 interaction dictionary, return the products expected from the implied interaction
listofproducts getNewExterns(const pairedgrouping& pair, const n1dict& nto1) {
	//Empty container
	listofproducts container;

	//If this grouping is invalid, add an empty vector
	if (!isGroupingValid(pair, nto1)) {
		container.push_back({});
	}
	else {
		//Get the grouped elements
		const grouping& groups{ pair.first };

		//Count how many groups there are
		int numgroups = groups.size();

		//Receive a list of possible products for each group
		std::vector<std::vector<P>> productlist;
		for (const std::vector<Particle>& group : groups) {
			productlist.push_back(getProducts(group, nto1));
		}

		//Aim : Produce all possible combinations of selecting one element for each group

		//Set up index counters for each of the groups
		int* indexmax{ new int[numgroups] {} };
		int* currentindex{ new int[numgroups] {} };
		for (int i{}; i < numgroups; ++i) {
			currentindex[i] = 0;
			indexmax[i] = productlist[i].size();
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