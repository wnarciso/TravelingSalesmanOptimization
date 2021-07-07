#include <cmath>
#include "City.h"

City::City()
{
	City(0, 0, 0);
}

City::City(int i, double newX, double newY)
	: ID(i), x(newX), y(newY)
{}

bool City::setxy(double newX, double newY)
{
	x = newX;
	y = newY;
	return true;
}

double City::getx() const
{
	return x;
}

double City::gety() const
{
	return y;
}

int City::getID() const
{
	return ID;
}

double City::distance(const City &c) const
{
	return sqrt(pow(x - c.getx(), 2) + pow(y - c.gety(), 2));
}

City& City::operator=(const City & c)
{
	if (this != &c)
	{
		ID = c.getID();
		x = c.getx();
		y = c.gety();
	}
	return *this;
}