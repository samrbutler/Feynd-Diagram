#include <set>
#include <map>
#include <iterator>
#include <iostream>
#include "Particles.h"

//Define a type alias for the n-to-1 interaction dictionary
using intdict = std::set<std::map<std::multiset<P>, P>>;

//Function prototype
intdict generateDictionary(const std::multiset<std::multiset<P>> interactions);
