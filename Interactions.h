// Interactions.h : Set up classes and types relating to interactions of particles

#pragma once

#include <set>
#include <map>
#include <iterator>
#include <iostream>

#include "Particles.h"

//Define a type alias for the n->1 interaction dictionary
using intdict = std::set<std::multimap<std::multiset<P>, P>>;

//Function prototype for generation of the n->1 particle dictionary
intdict generateDictionary(const std::multiset<std::multiset<P>>&);

//Define a vertex class
class Vertex : public Point {

};