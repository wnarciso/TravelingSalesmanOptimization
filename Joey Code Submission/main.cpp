#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <chrono>
#include <cmath>
#include <iomanip>
#include "City.h"
using namespace std;

// Using 48 US Capitals as cities
// Cities represetned as coords
// all cities adjacent

// some constants
const int NUMCITIES = 48;
const int KMAX = 2000;
const int DATAMAX = 100;
const double ACTUAL = 33523;
const int W = 20;

// all the coords of the states' capitals
const vector<double> coords = { 6734, 1453, 2233, 10, 5530, 1424, 401, 841, 3082, 1644, 7608, 4458, 7573, 3716, 7265, 1268, 6898, 1885, 1112, 2049, 5468, 2606,
								5989, 2873, 4706, 2674, 4612, 2035, 6347, 2683, 6107, 669, 7611, 5184, 7462, 3590, 7732, 4723, 5900, 3561, 4483, 3369, 6101, 1110,
								5199, 2182, 1633, 2809, 4307, 2322, 675, 1006, 7555, 4819, 7541, 3981, 3177, 756, 7352, 4506, 7545, 2801, 3245, 3305, 6426, 3173,
								4608, 1198, 23, 2216, 7248, 3779, 7762, 4595, 7392, 2244, 3484, 2829, 6271, 2135, 4985, 140, 1916, 1569, 7280, 4899, 7509, 3239,
								10, 2676, 6807, 2993, 5185, 3258, 3023, 1942 };

double square(double x)
{
	return x * x;
}

// return the mean of an array of doubles
double mean(const vector<double> arr)
{
	double total = 0;
	for (double d : arr)
	{
		total += d;
	}
	return total / static_cast<double> (arr.size());
}

// return the standard deviation for an array of doubles
double standard(const vector<double> arr)
{
	vector<double> stan;
	double m = mean(arr);

	for (double d : arr)
	{
		stan.push_back(square(d - m));
	}
	
	return sqrt(mean(stan));
}

// optimization methods for step by step
void swap(vector<City> & dest, int i, int j)				// swap 2 cities
{
	City temp;

	temp = dest.at(j);										// swap them
	dest.at(j) = dest.at(i);
	dest.at(i) = temp;

	return;
}

void shuffle(vector<City> & dest)							// shuffle the order of all the cities
{
	int j = 0;
	City temp;
	default_random_engine generator (chrono::system_clock::now().time_since_epoch().count());
	uniform_int_distribution<int> distribution(0, dest.size() - 1);

	for (int i = 0; i < dest.size(); i++)					// for each city
	{
		j = distribution(generator);						// choose a random other element
		if (j != i)											// allow for j = i
		{
			swap(dest, i, j);								// swap
		}
	}
	return;
}

double length(const vector<City> & dest)					// finds length of a given path
{
	double l = 0;

	for (int i = 0; i < dest.size() - 1; i++)				// traverse the list
		l += dest.at(i).distance(dest.at(i + 1));			// add up the lengths

	l += dest.at(0).distance(dest.at(dest.size() - 1));		// return trip

	return l;
}

// Takes a list of cities, randomizes them, then goes step by step
// if the step is better, keep it. Otherwise, go back
double StepByStep(vector<City> & dest)						// working with reference vector
{
	shuffle(dest);											// shuffle
	double l = length(dest), newl;							// get the starting length
	int c1, c2;

	default_random_engine generator (chrono::system_clock::now().time_since_epoch().count());
	uniform_int_distribution<int> distribution(0, dest.size() - 1);

	for (int i = 0; i < KMAX; i++)							// for max iterations...
	{
		c1 = distribution(generator);
		c2 = distribution(generator);
		while (c1 == c2)									// get another city
		{
			c2 = distribution(generator);
		}
		swap(dest, c1, c2);									// swap the two
		newl = length(dest);								// check the new length
		
		if (newl <= l)										// if better...
		{
			l = newl;										// keep it
		}
		else
		{
			swap(dest, c1, c2);								// otherwise, go back
		}
	}
	
	return l;												// return the best length found
}

// optimization function for nearest neighbor
// Start with city 0, then choose its nearest neighbor. Add that to the list
// Repeat until all cities have been reached.
double NearestNeighbor(vector<City> & dest)					// working with a reference vector
{
	int shortest = INT_MAX;									// keep track of the nearest neighbor and the dist to that neighbor
	int shortestCity;

	// selection sort
	for (int j = 1; j < dest.size(); j++)					// for each other city
	{
		shortest = INT_MAX;
		for (int i = j; i < dest.size(); i++)				// go through unsorted cities
		{
			if (dest.at(i).distance(dest.at(0)) < shortest)	// if we founf the new nearest
			{
				shortestCity = i;							// keep track of it
				shortest = dest.at(i).distance(dest.at(0));
			}
		}
		swap(dest, j, shortestCity);						// sort the nearest neighbor
	}
	
	return length(dest);									// return the length
}

int main(void)
{	
	vector<City> destinations;								// all the places we need to visit
	vector<double> pathlengths;

	ofstream outf;

	for (int i = 0; i < NUMCITIES; i++)						// generate NUMCITIES cities
	{
		destinations.push_back(City{ i, coords.at(2*i), coords.at(2*i+1) });
	}
	
	vector<City> d1 = destinations, dmin;
	int min = 0;
	for (int i = 0; i < DATAMAX; i++)						// run step by step for datamax iterations
	{
		pathlengths.push_back(StepByStep(d1));				// d1 gets shuffled each time, so we can reuse it
		if (pathlengths.at(min) > pathlengths.at(i))
		{
			min = i;										// keep track of the minimum
			dmin = d1;
		}
	}

	// define some results
	vector<City> d2 = destinations;
	double nnRes = NearestNeighbor(d2);
	double sbsRes = mean(pathlengths);

	outf.open("results.txt", fstream::out);
	if (!outf)
	{
		cout << "Error: could not open output file\n";
		return -1;
	}

	outf << fixed << setprecision(2)
		<< "Results:\n"
		<< "=========================================================\n"
		<< "Actual:\n"
		<< setw(W) << "Path Length: " << ACTUAL << endl << endl
		<< "=========================================================\n"
		<< "Step By Btep:\n"
		<< setw(W) << "Optimal Path: ";

	for (City c : dmin)
	{
		outf << c.getID() << " ";
	}

	outf << endl 
		<< setw(W) << "Iterations: " << DATAMAX << endl
		<< setw(W) << "Mean Path Length: " << sbsRes << " += " << standard(pathlengths) << endl
		<< setw(W) << "Percent Error: " << (sbsRes / ACTUAL) * 100 << "%\n\n"
		<< "=========================================================\n"
		<< "Nearest Neighbor:\n"
		<< setw(W) << "Optimal Path: ";

	for(City c : d2)
	{
		outf << c.getID() << " ";
	}

	outf << endl
		<< setw(W) << "Path Length: " << nnRes << endl
		<< setw(W) << "Percent Error: " << (nnRes / ACTUAL) * 100 << "%";

	outf.close();
	cout << "Bye!\n";
	return 0;
}