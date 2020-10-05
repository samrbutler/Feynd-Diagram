/*Model.h : DEFINE THE MODEL IN THIS FILE - OBJECTS MARKED *** NEED MODIFICATION
	Enum classes
		- ParticleType ***
			> the names of particles appearing in the model
	Type aliases
		- n0dict
			> A dictionary for n->0 interactions
		- n1dict
			> A dictionary for n->1 interactions
		- P
			> an alias for the ParticleType enum class
	Global constants
		- Interactions ***
			> define all interactions of the model
		- AntiParticleDict ***
			> define the antiparticles for every particle in the model
	Function declarations
		- getAntiParticle
		- generateDictionary
*/

#pragma once

#include <iterator>
#include <map>
#include <set>
#include <tuple>
#include <vector>


//The names of particles appearing in the model
namespace Model {
	enum class ParticleType {
		phi,
		psi,
		antipsi,
		chi
	};
}

//Alias for the ParticleType enum class
using P = Model::ParticleType;
//A dictionary for n->0 interactions
using n0dict = std::multiset<std::multiset<P>>;
//A dictionary for n->1 interactions
using n1dict = std::set<std::pair<std::multiset<P>, P>>;

namespace Model {
	//Define the n->0 dictionary
	inline const n0dict Interactions{
		{P::phi,P::psi,P::antipsi},
		{P::chi,P::psi,P::antipsi}
	};

	//The antiparticles for every particle in the model
	inline const std::map<P, P> AntiParticleDict{
		{P::phi		,P::phi},
		{P::psi		,P::antipsi},
		{P::antipsi	,P::psi},
		{P::chi		,P::chi}
	};
}

P getAntiParticle(const P part);
n1dict generateDictionary(const n0dict& = Model::Interactions);

namespace Model {
	inline const n1dict NTO1{ generateDictionary() };
}