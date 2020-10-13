/*Model.h : Define the following model-specific quantities:
*	Enum classes
*		- ParticleType
*			> the names of particles appearing in the model
*	Global (inline) constants
*		- n_to_0
*			> define all interactions of the model
*		- AntiParticleDict
*			> define the antiparticles for every particle in the model
*   Update also the overloaded operator>> for ParticleType in Model.cpp
*/

#pragma once

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <set>
#include <tuple>
#include <vector>


//The names of particles appearing in the model
namespace Model {
	enum class ParticleType {
		phi,
		psi,
		antipsi,
	};
}

//Alias for the ParticleType enum class
using P = Model::ParticleType;
//A dictionary for n->0 interactions
using n0dict = std::multiset<std::multiset<P>>;
//A dictionary for n->1 interactions
using n1dict = std::set<std::pair<std::multiset<P>, P>>;
//A dictionary for n->m interactions
using loopdict = std::set<std::pair<std::multiset<P>, std::multiset<P>>>;

namespace Model {
	//Define the n->0 dictionary
	inline const n0dict n_to_0{
		{P::phi,P::phi,P::phi,P::phi},
		{P::phi,P::psi,P::antipsi}
	};

	//The antiparticles for every particle in the model
	inline const std::map<P, P> AntiParticleDict{
		{P::phi		,P::phi},
		{P::psi		,P::antipsi},
		{P::antipsi	,P::psi}
	};
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