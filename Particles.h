#pragma once
#include <map>

//Define particle names
enum class ParticleType {
	phi,
	psi,
	antipsi,
	chi
};

using P = ParticleType;

inline const std::map<P, P> ParticleDict{
	{P::phi		,P::phi},
	{P::psi		,P::antipsi},
	{P::antipsi	,P::psi}
};