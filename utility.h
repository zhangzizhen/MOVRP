#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>

const int MAXN = 510;	// maximum number of customers
const int MAXV = 22;	// maximum number of vehicles
const int MAXC = 22;	// maximum number of companies
const int MAXP = 200;	// maximum population size

// coordinate
struct Point {
	double x, y;
};

// route infomation
struct Route {
	int nv;
	double load;
	double length;
	double profit;
	int v[MAXN];
};

// solution structure
struct Solution {
	double fairness;
	double length;
	int tour[MAXN];
	Route r[MAXV];
};


// calculate the distance between two points
double dist(Point &p1, Point &p2);

// calculate the angle between two points
double angleBetween(Point &p1, Point &p2, Point &p0);

// check whether s1 dominates s2
bool is_dominate(Solution &s1, Solution &s2);

// check whether s1 == s2
bool is_same(Solution &s1, Solution &s2);

// calculate the load of a route
void calculate_route_load(Route &r);

// calculate the length of a route
void calculate_route_length(Route &r);

// calculate the profit of a route
void calculate_route_profit(Route &r);

// calculate the total length of a solution
void calculate_solution_length(Solution &s);

// calculate the fairness of a solution
void calculate_solution_fairness(Solution &s);

// build a giant tour from a set of routes
void make_giant_tour(Solution &s);

// Get a random value in [x,y)
double random(double x, double y);

// output solution
void output(Solution &s);

void output(FILE *file, Solution &s);

#endif
