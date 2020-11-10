#include "Point.h"

//Initialise the unique point ID counter
int Point::next_id{ 0 };

//Overload operator< to compare point IDs
bool operator<(const Point& point1, const Point& point2)
{
	return (point1.getID() < point2.getID());
}

//Overload operator> to compare point IDs
bool operator>(const Point& point1, const Point& point2)
{
	return (point1.getID() > point2.getID());
}

//Overload operator== to compare point IDs
bool operator==(const Point& point1, const Point& point2)
{
	return (point1.getID() == point2.getID());
}

//Overload operator!= to compare point IDs
bool operator!=(const Point& point1, const Point& point2)
{
	return (point1.getID() != point2.getID());
}