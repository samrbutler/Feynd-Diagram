#pragma once
#include <vector>
#include <set>
#include <tuple>
#include "Particles.h"

//Define a basic point class with unique identifier
class Point {
private:
	static int next_id;
public:
	int m_id{};

	Point();

};

//Set up vertex class to equip a point with legs
class Vertex : public Point {
public:
	int m_numlegs{};
	bool m_connected{};
	std::vector<P> m_available{};
	std::vector<Vertex> m_connections{};
	Vertex() = default;
	Vertex(std::vector<P>&);
};

//Define a class for external particles
class External : public Point {
public:
	P m_ptype;

	External() = delete;
	External(P&);

};

class Diagram {
public:
	std::vector<External> m_externs{};
	std::vector<Vertex> m_vertices{};
	Diagram(std::vector<External>&);
};