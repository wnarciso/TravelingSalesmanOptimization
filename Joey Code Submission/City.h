#ifndef CITY_H
#define CITY_H

class City
{
private:
	int ID;									// "name" of the city also location in destinations vector
	double x;								// some coords
	double y;

public:
	City();
	City(int, double, double);

	bool setxy(double, double);				// set the coords
	double getx() const;					// get coords one at a time
	double gety() const;
	int getID() const;						// get the ID of the city

	double distance(const City &) const;	// distance to city c

	City& operator=(const City&);			// assignment operator
};

#endif