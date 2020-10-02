// Diagram.h : Set up diagram construction

#pragma once

#include "Interactions.h"
#include "Particles.h"

#include <algorithm>
#include <utility>
#include <vector>

//Define the type aliases for groupings
using grouping = std::vector<std::vector<Particle>>;
using pairedgrouping = std::pair<grouping, std::vector<Particle>> ;
using listofgroupings = std::vector<grouping>;

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

};

std::vector<pairedgrouping> getSubsets(std::vector<Particle> &input);

bool operator<(const std::vector<Particle>& vec1, const std::vector<Particle>& vec2);
bool operator==(std::vector<std::vector<Particle>>& part1, std::vector<std::vector<Particle>>& part2);
bool operator!=(std::vector<Particle>& part1, std::vector<Particle>& part2);

listofgroupings getGroupings(std::pair<std::vector<std::vector<Particle>>, std::vector<Particle>> &pairup);
