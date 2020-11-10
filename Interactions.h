#pragma once

#include "Groups.h"
#include "Model.h"
#include "Particle.h"
#include "Vertex.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <vector>

//A list of the products following the interactions prescribed by a grouping
using listofproducts = std::vector<std::vector<P>>;

std::multiset<P> vec2multiset(const std::vector<Particle>& group);

bool isGroupingValid(const pairedgrouping& pair, const n1dict& dictionary = Model::n_to_1);

std::vector<P> getProducts(const std::vector<Particle>& group, const n1dict& dictionary = Model::n_to_1);

listofproducts getNewExterns(const pairedgrouping& pair, const n1dict& nto1 = Model::n_to_1);