/*Model.h : Define the following model-specific quantities at compile-time:
*		- ParticleType
*			> the identifiers of particles appearing in the model
*		- particle_names
*			> define display names for every particle in the model
*		- AntiParticleDict
*			> define the antiparticles for every particle in the model
*		- n_to_0
*			> define all interactions of the model
*/

#pragma once

#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <set>
#include <tuple>
#include <vector>

//DO NOT EDIT
namespace Model {
	enum class ParticleType;
}
using P = Model::ParticleType;

//A dictionary for n->0 interactions
using n0dict = std::multiset<std::multiset<P>>;
//A dictionary for n->0 interactions
using n0dict = std::multiset<std::multiset<P>>;
//A dictionary for n->1 interactions
using n1dict = std::set<std::pair<std::multiset<P>, P>>;
//A dictionary for n->m interactions
using loopdict = std::set<std::pair<std::multiset<P>, std::multiset<P>>>;

//EDIT ONLY THIS SECTION
namespace Model {
	//Define all particles in the theory
	enum class ParticleType {
		phi,
		chi,
		psi,
		antipsi,
		END				//Reference element to calculate enum class size: this should not be directly referenced by the user
	};
	//Provide display names for the above particles
	inline const std::array<std::string, static_cast<int>(ParticleType::END)> particle_names{
		"Phi",
		"Chi",
		"Psi",
		"Antipsi"
	};
	//Provide the antiparticles for every particle in the model
	inline const std::map<P, P> AntiParticleDict{
		{P::chi		,P::chi},
		{P::phi		,P::phi},
		{P::psi		,P::antipsi},
		{P::antipsi	,P::psi}
	};
	//Define the n->0 interaction dictionary
	inline const n0dict n_to_0{
		//{P::phi,P::phi,P::phi,P::phi},
		//{P::phi,P::phi,P::chi,P::chi},
		//{P::phi,P::psi,P::antipsi}
		{P::psi,P::antipsi,P::phi,P::phi},
		{P::phi,P::phi,P::chi},
		{P::chi,P::psi,P::antipsi}
	};
}

//DO NOT EDIT FROM HERE ONWARDS

const std::array<P, static_cast<int>(P::END)> getPartTypes();
const int getMaxLegs();

namespace Model {
	inline const std::array<P, static_cast<int>(P::END)> all_ParticleTypes{ getPartTypes() };
	inline const int max_legs{ getMaxLegs() };
}

std::ostream& operator<<(std::ostream& out, const P part);
P getAntiParticle(const P part);
n1dict generateN1Dictionary(const n0dict & = Model::n_to_0);

namespace Model {
	inline const n1dict n_to_1{ generateN1Dictionary() };
}

loopdict getLoopDictionary(const n1dict& inters = Model::n_to_1);

namespace Model {
	inline const loopdict n_to_many{ getLoopDictionary() };
}