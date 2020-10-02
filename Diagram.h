// Diagram.h : Set up diagram construction

#pragma once

#include <vector>
#include <algorithm>
#include <utility>

#include "Particles.h"
#include "Interactions.h"

//Define the type alias for a grouping
using grouping = std::vector<std::pair<std::vector<std::vector<Particle>>, std::vector<Particle>>>;



//Define a class for diagrams
class Diagram {
	
	//External particles
	std::vector<Particle> m_externs;
	//Internal vertices
	std::vector<Vertex> m_vertices;

public:

	//Private member access
	std::vector<Particle>& getExterns() { return m_externs; }
	std::vector<Vertex>& getVertices() { return m_vertices; }

	//Default constructor with initialisation list
	Diagram(std::vector<Particle>& externs) : m_externs{ externs }, m_vertices{}
	{
		//No explicit constructor
	}

	//Function to group up vertices
	void groupify(const n0dict& nto0, const n1dict& nto1);

};

grouping getSubsets(std::vector<Particle> &input);

bool operator<(const std::vector<Particle>& vec1, const std::vector<Particle>& vec2);
bool operator==(std::vector<std::vector<Particle>>& part1, std::vector<std::vector<Particle>>& part2);
bool operator!=(std::vector<Particle>& part1, std::vector<Particle>& part2);

std::vector<std::vector<std::vector<Particle>>> getGroupings(std::pair<std::vector<std::vector<Particle>>, std::vector<Particle>> &pairup);
