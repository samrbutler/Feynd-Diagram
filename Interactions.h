#include <set>
#include <map>
#include <iterator>
#include <iostream>
#include "Particles.h"

using intdict = std::set<std::map<std::multiset<P>, P>>;
intdict generateDictionary(const std::multiset<std::multiset<P>> interactions);
