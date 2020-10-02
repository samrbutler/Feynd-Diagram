// Diagram.h : Set up diagram construction

#pragma once

#include "Groups.h"
#include "Interactions.h"
#include "Particles.h"

#include <vector>


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

	bool isVertex(n0dict& dictionary);

};
