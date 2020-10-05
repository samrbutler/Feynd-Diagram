/*Diagram.h :
	Classes
		- Diagram
			> Define a class for a diagram
		- Process (inherits Diagram)
			> Extend the diagram class to allow initial setup of a process
	Function declarations
		- operator<<
		- connect
*/

#pragma once

#include "Groups.h"
#include "Interactions.h"
#include "Model.h"
#include "Particles.h"

#include <iterator>
#include <vector>


//Define a class for a diagram
class Diagram {

protected:	

	//External particles
	std::vector<Particle> m_externs;
	//Internal vertices
	std::vector<Vertex> m_vertices;

public:

	//Return the external particles
	const std::vector<Particle>& getExterns() const { return m_externs; }
	//Return the vertices of the diagram
	const std::vector<Vertex>& getVertices() const { return m_vertices; }

	//Default constructor of any empty diagram
	Diagram() : m_externs{}, m_vertices{} {};

	//Construct a diagram from a vector of particles
	Diagram(std::vector<Particle>& externs) : m_externs{ externs }, m_vertices{ } {}

	Diagram(std::vector<Particle>& externs, Vertex vertex);

	//Add a vertex to the diagram
	void addVertex(Vertex vert) { m_vertices.push_back(vert); }
	
	void addVertices(std::vector<Vertex> verts);

	bool isVertex(const n0dict& dictionary);
	
};

//Extend the diagram class to allow initial setup of a process
class Process : public Diagram {

	//Incoming particle names
	std::vector<P> m_incoming;
	//Outgoing particle names
	std::vector<P> m_outgoing;

public:

	//Create a process from incoming and outgoing particle names
	Process(std::vector<P> incoming, std::vector<P> outgoing);
};

std::ostream& operator<<(std::ostream& out, const Diagram& diag);

std::vector<Diagram> connect(Diagram& diag,const n0dict& nto0 = Model::Interactions, const n1dict& nto1 = Model::NTO1,bool debug = false);