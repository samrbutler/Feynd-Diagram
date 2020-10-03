// Diagram.h : Set up diagram construction

#pragma once

#include "Groups.h"
#include "Interactions.h"
#include "Particles.h"

#include <vector>


//Define a class for diagrams
class Diagram {
protected:	
	//External particles
	std::vector<Particle> m_externs;
	//Internal vertices
	std::vector<Vertex> m_vertices;

public:

	//Private member access
	std::vector<Particle>& getExterns() { return m_externs; }
	std::vector<Vertex>& getVertices() { return m_vertices; }

	//Declare a default constructor for inheritance
	Diagram() = default;

	//Constructor with initialisation list
	Diagram(std::vector<Particle>& externs) : m_externs{ externs }, m_vertices{ }
	{
		//No explicit constructor
	}

	//Construct with vertices
	Diagram(std::vector<Particle>& externs, Vertex vertex);
	
	bool isVertex(const n0dict& dictionary);
};

class Process : public Diagram {

	//Store incoming and outgoing particle names
	std::vector<P> m_incoming;
	std::vector<P> m_outgoing;

public:
	//Create a process from incoming and outgoing particle names
	Process(std::vector<P>& incoming, std::vector<P>& outgoing);
};

std::vector<Diagram> connect(Diagram& proc,const n0dict& nto0, const n1dict& nto1);
