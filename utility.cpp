#include <cmath>
#include <cstdio>
#include <cstdlib>
#include "utility.h"
using namespace std;

//extern const int MAXN;

const double PI = acos(-1);
const double eps = 1e-8;

/********************************
Instance parameters
********************************/
int n;	// number of customers
int V;	// number of vehicles
int C;	// capacity
int nc;	// number of companies
int company[MAXV];		// company of a vehicle
int vc[MAXC];
double cap[MAXV + 1];	// capacity of vehicles
double c[MAXN][MAXN];	// distance (cost) matrix
double d[MAXN];			// demand of customer
double reward[MAXN];	// reward
Point p[MAXN];			// coordinate of customers

double alpha = 1;
double beta = 1;
double gmin_length;
double gmin_fairness;

/********************************
Algorithm parameters
********************************/
int POP_SIZE = 200;			// Population size
double Pr = 1.0;			// Recombination probability
double Pm = 0.05;           // Mutation probability
double Pls = 0.2;			// Local search probability
int ls_iter = 3000;			// Local search iteration
double T = 600;				// Execution time (second)
double tau = 0.5;			// Tournament size
//int seed = 1;				// Random seed


double dist(Point &p1, Point &p2)
{
	return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

double angleBetween(Point &p1, Point &p2, Point &p0)
{
    double sin = (p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y);  
    double cos = (p1.x - p0.x) * (p2.x - p0.x) + (p1.y - p0.y) * (p2.y - p0.y);
    double ret = atan2(sin, cos) * (180 / PI);
    return (ret >= 0) ? ret : (ret + 360);
}

bool is_dominate(Solution &s1, Solution &s2)
{
	if ((int)(s1.length / 100000) < (int)(s2.length / 100000)) return true;
	if ((int)(s1.length / 100000) > (int)(s2.length / 100000)) return false;

	if (s1.fairness > s2.fairness + eps || s1.length > s2.length + eps) return false;
	return (s1.fairness + eps < s2.fairness || s1.length + eps < s2.length);
}

bool is_same(Solution &s1, Solution &s2)
{
	return (fabs(s1.fairness - s2.fairness) < eps && fabs(s1.length - s2.length) < eps);
}

void calculate_route_load(Route &r)
{
	r.load = 0;
	for (int i = 1; i < r.nv; i++) {
		r.load += d[r.v[i]];
	}
}

void calculate_route_length(Route &r)
{
	r.length = 0;
	for (int i = 0; i < r.nv - 1; i++) {
		r.length += c[r.v[i]][r.v[i + 1]];
	}
	r.length += c[r.v[r.nv - 1]][0];
}

void calculate_route_profit(Route &r)
{
	r.profit = -beta * r.length;
	for (int i = 1; i < r.nv; i++) {
		r.profit += alpha * reward[r.v[i]];
	}
}

void calculate_solution_length(Solution &s)
{
	s.length = 0;
	for (int i = 0; i < V; i++)
		s.length += s.r[i].length;
	s.length += s.r[V].length * 100000;
}

void calculate_solution_fairness(Solution &s)
{
	int i;
	double pf[MAXC];
	for (i = 0; i < nc; i++)
		pf[i] = 0;
	for (i = 0; i < V; i++)
		pf[company[i]] += s.r[i].profit / vc[company[i]];
	double maxp = -1e10;
//~~
//double minp = 1e10;
	for (i = 0; i < nc; i++) {
		if (-pf[i] > maxp) maxp = -pf[i];
// ~~~
//if (-pf[i] < minp) minp = -pf[i];
	}

	s.fairness = maxp;
//	s.fairness = minp;
// ~~~
//s.fairness = maxp - minp;

}

void make_giant_tour(Solution &s)
{
	int k = 0;
	s.tour[k++] = 0;
	for (int i = 0; i <= V; i++)
	for (int j = 1; j < s.r[i].nv; j++)
		s.tour[k++] = s.r[i].v[j];
	s.tour[k] = 0;
}

double random(double x, double y)
{
	return (double)rand() / RAND_MAX * (y - x) + x;
}

void output(Solution &s)
{
	int i, j;

	printf("%.8lf %.8lf\n", s.length, s.fairness);
	for (i = 0; i < n; i++)
		printf("%d, ", s.tour[i]);
	printf("\n");
	for (i = 0; i <= V; i++) {
		printf("V%d %d (%.2lf %.2lf %.2lf):", i, company[i], s.r[i].load, s.r[i].length, s.r[i].profit);
		for (j = 0; j <= s.r[i].nv; j++)
			printf(" %d", s.r[i].v[j]);
		printf("\n");
	}
	fflush(stdout);
}

void output(FILE *file, Solution &s)
{
	int i, j;
	fprintf(file, "%.8lf %.8lf\n", s.length, s.fairness);
	for (i = 0; i < n; i++)
		fprintf(file, "%d, ", s.tour[i]);
	fprintf(file, "\n");
	for (i = 0; i <= V; i++) {
		fprintf(file, "V%d %d (%.2lf %.2lf %.2lf):", i, company[i], s.r[i].load, s.r[i].length, s.r[i].profit);
		for (j = 0; j <= s.r[i].nv; j++)
			fprintf(file, " %d", s.r[i].v[j]);
		fprintf(file, "\n");
	}
}
