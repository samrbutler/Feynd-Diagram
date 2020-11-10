#pragma once

#include "UserModel.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <set>
#include <tuple>
#include <vector>

//A dictionary for n->1 interactions
using n1dict = std::set<std::pair<std::multiset<P>, P>>;

//A dictionary for n->m interactions
using loopdict = std::set<std::pair<std::multiset<P>, std::multiset<P>>>;

const std::array<P, static_cast<int>(P::END)> getPartTypes();

const int getMaxLegs();

namespace Model
{
	inline const std::array<P, static_cast<int>(P::END)> all_particle_types{ getPartTypes() };
	inline const int max_legs{ getMaxLegs() };
}

std::ostream& operator<<(std::ostream& out, const P part);

P getAntiParticle(const P part);

n1dict generateN1Dictionary(const n0dict & = Model::n_to_0);

namespace Model
{
	inline const n1dict n_to_1{ generateN1Dictionary() };
}

loopdict getLoopDictionary(const n1dict& inters = Model::n_to_1);

namespace Model
{
	inline const loopdict n_to_many{ getLoopDictionary() };
}