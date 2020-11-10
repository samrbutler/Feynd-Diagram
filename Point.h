#pragma once

//A point container class
class Point
{
	//Consecutive ID counter
	static int next_id;
	int m_id;

public:

	//Default constructor to initialise the ID
	Point() : m_id{ ++next_id } {}

	//Virtual destructor
	virtual ~Point() {}

	//Return the ID of the point
	int getID() const { return m_id; }

	//Set the ID of the point
	void setID(const int id) { m_id = id; }

	//Return the current ID counter
	static int getStaticID() { return next_id; }

	//Set the current ID counter
	static void setStaticID(const int x) { next_id = x; }
};

//Overload operator< to compare point IDs
bool operator<(const Point& point1, const Point& point2);

//Overload operator> to compare point IDs
bool operator>(const Point& point1, const Point& point2);

//Overload operator== to compare point IDs
bool operator==(const Point& point1, const Point& point2);

//Overload operator!= to compare point IDs
bool operator!=(const Point& point1, const Point& point2);