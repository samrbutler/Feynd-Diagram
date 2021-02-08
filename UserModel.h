#pragma once

#include <array>
#include <map>
#include <string>
#include <set>

//DO NOT EDIT
namespace Model
{
	enum class ParticleType;
}

using P = Model::ParticleType;

using n0dict = std::multiset<std::multiset<P>>;

/*UserModel.h : Define the following model-specific quantities:
*		- ParticleType
*			> the identifiers of particles appearing in the model
*		- particle_names
*			> define display names for every particle in the model
*		- AntiParticleDict
*			> define the antiparticles for every particle in the model
*		- n_to_0
*			> define all interactions of the model
*/

//EDIT FROM HERE

namespace Model
{
	//Define all particles in the theory
	enum class ParticleType
	{
		phi,
		chi,
		psi,
		antipsi,
		END				//Reference element to calculate enum class size: this should not be directly referenced by the user
	};

	//Provide display names for the above particles
	inline const std::array<std::string, static_cast<int>(P::END)> particle_names
	{
		"Phi",
		"Chi",
		"Psi",
		"Antipsi"
	};

	//Provide the antiparticles for every particle in the model
	inline const std::map<P, P> AntiParticleDict
	{
		{P::chi		,P::chi},
		{P::phi		,P::phi},
		{P::psi		,P::antipsi},
		{P::antipsi	,P::psi}
	};

	//Define the n->0 interaction dictionary
	inline const n0dict n_to_0
	{
		//{P::phi,P::phi,P::phi,P::phi},
		//{P::phi,P::phi,P::chi,P::chi},
		{P::phi,P::psi,P::antipsi}
		//{P::phi,P::phi,P::phi}
	};
}