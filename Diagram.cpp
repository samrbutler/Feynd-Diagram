// Diagram.cpp : Code relevant to diagram construction and manipulation

#include <vector>
#include <utility>
#include <bits.h>
#include "Diagram.h"
#include "Particles.h"
#include "Interactions.h"

//Defines function groupify to take interaction dictionariesand a vector of Particles, returning all possible allowed groupings
grouping Diagram::groupify(const n0dict& nto0, const n1dict& nto1) {
	std::vector<Particle> nodes = m_externs;
	auto pairups = getSubsetPairings(nodes);


}

std::vector<std::pair<std::vector<Particle>, std::vector<Particle>>> getSubsetPairings(std::vector<Particle> input) {
	std::vector<std::pair<std::vector<Particle>, std::vector<Particle>>> pairings{};
	for (int i{}; i < pow(2, input.size()); ++i) {
		std::vector<Particle> subset{};
		std::vector<Particle> notsubset{};
		for (int j{}; j < static_cast<int>(input.size()); ++j) {
			if ((i & (1 << j)) != 0)
				subset.push_back(input[j]);
			else
				notsubset.push_back(input[j]);
		}
		auto pairup{ std::make_pair(subset, notsubset) };
		pairings.push_back(pairup);
	}
	
}