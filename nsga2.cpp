#include <cstring>
#include <string>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "initial.h"
#include "dp_split.h"
#include "nsga2.h"
#include "recombination.h"
#include "mutation.h"
#include "ls.h"
#include "lns.h"
using namespace std;

const double inf = 1e30;

extern const int MAXP;

extern int n;
extern int V;
extern int C;
extern double gmin_length;
extern double gmin_fairness;

extern int POP_SIZE;		// Population size
extern double Pr;			// Recombination probability
extern double Pm;			// Mutation probability
extern double tau;
extern double Pls;			// Local search probability
extern int ls_iter;
extern double T;			// Execution time
extern int seed;			// Random seed

struct NSGA2 {
	int n;
	int nS;
	int rank;
	double dis;
	int S[MAXP * 2];
	Solution s;
	bool operator < (const NSGA2 &other) const {
		return s.length < other.s.length;
	}
};

struct Front {
	int f[MAXP * 2];
	int n;
};

struct Crowding {
	int a;
	double f;
	bool operator < (const Crowding &c) const {
		return f < c.f;
	}
};

int n_par;
int n_chi;

NSGA2 population[MAXP * 2];
NSGA2 p1[MAXP];

Front front[MAXP * 2];
Crowding crowding[MAXP * 2];
int front_size;

/*
 * Fast-non-dominated-sort
*/
void nsga2_fnds()
{
	int h, i, j, k, l;
	front[1].n = 0;
	for (i = 0; i < n_par + n_chi; i++) {
		population[i].n = 0;
		population[i].nS = 0;
		for (j = 0; j < n_par + n_chi; j++)
		if (i != j) {
			if (is_dominate(population[i].s, population[j].s)) {
				population[i].S[population[i].nS++] = j;
			}
			else if (is_dominate(population[j].s, population[i].s)) {
				population[i].n++;
			}
		}
		if (population[i].n == 0) {
			population[i].rank = 1;
			front[1].f[front[1].n++] = i;
		}
	}
	for (i = 1; front[i].n != 0; i++) {
 		front[i + 1].n = 0;
		for (j = 0; j < front[i].n; j++) {
			k = front[i].f[j];
			for (l = 0; l < population[k].nS; l++) {
				h = population[k].S[l];
				population[h].n--;
				if (population[h].n == 0) {
					population[h].rank = i + 1;
					front[i + 1].f[front[i + 1].n++] = h;
				}
			}
		}
	}
	front_size = i - 1;
}


/*
 * Crowding-distance-assignment
*/
void nsga2_cda(Front &front)
{
	int i, j;
	for (i = 0; i < front.n; i++)
		population[front.f[i]].dis = 0;

	for (i = 0; i < 2; i++) {
		for (j = 0; j < front.n; j++) {
			crowding[j].f = ((i == 0) ? population[front.f[j]].s.fairness : population[front.f[j]].s.length);
			crowding[j].a = front.f[j];
		}
		sort(crowding, crowding + front.n);
		population[crowding[0].a].dis = inf;
		population[crowding[front.n - 1].a].dis = inf;
		for (j = 1; j < front.n - 1; j++)
		if (population[crowding[j].a].dis < inf)
			population[crowding[j].a].dis += (crowding[j + 1].f - crowding[j - 1].f) / (crowding[front.n - 1].f - crowding[0].f);
	}
}

/*
 * filter the same objective solutions
*/
void nsga2_unique()
{
	int i, j, k;
	k = n_par;
	for (i = n_par; i < n_par + n_chi; i++) {
		for (j = 0; j < i; j++)
			if (is_same(population[i].s, population[j].s)) break;
		if (j == i) population[k++] = population[i];
	}
	n_chi = k - n_par;
}

/*
 *
*/
bool nsga2_cmp(int x, int y)
{
	return (population[x].rank < population[y].rank ||
			(population[x].rank == population[y].rank &&
			 population[x].dis > population[y].dis));
}

void evaluate(Solution &s)
{
	Solution ss = s;
	split_min_length(s);
	split_min_fairness(ss);
	if (is_dominate(s, ss)) return;
	if (is_dominate(ss, s)) {
		s = ss;
		return;
	}
//	if (rand() % 2) split_min_length(s);
//		else split_min_fairness(s);
}


/*
 * Recombine to produce a new population
*/
void nsga2_recombination()
{
	int cd = (int)(Pr * n_par);
	int a[MAXP];
	for (int i = 0; i < n_par; i++) a[i] = i;
	std::random_shuffle(a, a + n_par);

	n_chi = 0;
	for (int i = 0; i < cd; i += 2)
	if (i + 1 < n_par) {
		recombination_operator(population[a[i]].s, population[a[i + 1]].s, population[n_par + i].s, population[n_par + i + 1].s);
		evaluate(population[n_par + i].s);
		evaluate(population[n_par + i + 1].s);
		n_chi += 2;
	}
	else 
//	for (int i = 0; i < cd; i++)
	{
		population[n_par + i] = population[a[i]];
		n_chi++;
	}

}

/*
*       Mutate population
*/
void nsga2_mutation()
{
	int i, j;
	for (i = n_par; i < n_par + n_chi; i++) {
		Solution &s = population[i].s;
		for (j = 1; j < n; j++)
			if (rand() < Pm * RAND_MAX) mutation_operator(s);
		evaluate(s);
	}
}

/*
void nsga2_local_search()
{
	if (n_chi == 0) return;

	int i, j;
	int ls = n_chi * Pls;
	double weight[2];
	for (i = 0; i < ls; i++) {
		j = rand() % n_chi + n_par;

		Solution &s = population[rand() % n_chi + n_par].s;
		LNS(s);

		{
		weight[0] = 1; weight[1] = 0;
		Solution &s = population[rand() % n_chi + n_par].s;
		local_search_operator(s, weight, ls_iter);
		}

		{
		weight[0] = 0; weight[1] = 1;
		Solution &s = population[rand() % n_chi + n_par].s;
		local_search_operator(s, weight, ls_iter);
		}

	}
}
*/


double get_obj_max(int j)
{
	double ret = -1e10;
	for (int i = 0; i < n_par + n_chi; i++) {
		Solution &s = population[i].s;
		double f = ((j == 0) ? s.length : s.fairness);
		if (f > ret) ret = f;
	}
	return ret;
}

double get_obj_min(int j)
{
	double ret = 1e10;
	for (int i = 0; i < n_par + n_chi; i++) {
		Solution &s = population[i].s;
		double f = ((j == 0) ? s.length : s.fairness);
		if (f < ret) ret = f;
	}
	return ret;
}

Solution& tournament(double weight[])
{
	int i;
	int t = int(tau * n_chi);
	double mf = inf;
	Solution *ret;
	for (i = 0; i < t; i++) {
		Solution &s = population[rand() % n_chi + n_par].s;
		double func = s.length * weight[0] + s.fairness * weight[1];
		if (func < mf) {
			mf = func;
			ret = &s;
		}
	}
	return (*ret);
}

void nsga2_local_search()
{
	if (n_chi == 0) return;

	int i;
	int ls = n_chi * Pls;
	double weight[2];
	for (i = 0; i < ls; i++) {
//		double mx;
//		mx = get_obj_max(0) - get_obj_min(0);
//		weight[0] = ((mx == 0) ? 0 : random(0, 1 / mx));
//		mx = get_obj_max(1) - get_obj_min(1);
//		weight[1] = ((mx == 0) ? 0 : random(0, 1 / mx));

		{
		weight[0] = 1; weight[1] = 0;
		Solution &s = tournament(weight);
LNS(s);
		local_search_operator(s, weight, ls_iter);
		}

		{
		weight[0] = 0; weight[1] = 1;
		Solution &s = tournament(weight);
LNS(s);
		local_search_operator(s, weight, ls_iter);
		}

	}
}


/*
 * NSGA2 main procedure
*/
void nsga2(string filename)
{
	FILE *file = fopen(filename.c_str(), "w");

	int t, i, j;
	n_par = POP_SIZE;
	n_chi = 0;

	gmin_fairness = inf;
	gmin_length = inf;

	for (i = 0; i < n_par; i++) {
		memset(&population[i], 0, sizeof(population[i]));
		generate(population[i].s);
		if (population[i].s.fairness < gmin_fairness) gmin_fairness = population[i].s.fairness;
		if (population[i].s.length < gmin_length) gmin_length = population[i].s.length;
	}

	for (i = 0; i < n_par; i++)
		p1[i] = population[i];

	clock_t start = clock();
	clock_t finish;


	int Gen = 100;
	for (t = 0; t < Gen; t++) {
		printf("%d\n", t);
		fflush(stdout);
		nsga2_recombination();
//		nsga2_mutation();
		nsga2_local_search();

		// remember to evaluate each chromosome in the population before this stage
		nsga2_unique();
		nsga2_fnds();

		int np1 = 0;
		i = 1;
		while (np1 + front[i].n < n_par) {
			for (int j = 0; j < front[i].n; j++)
				p1[np1++] = population[front[i].f[j]];
			i++;
		}
		nsga2_cda(front[i]);
		sort(front[i].f, front[i].f + front[i].n, nsga2_cmp);
		for (j = 0; np1 < n_par; j++)
			p1[np1++] = population[front[i].f[j]];

		for (i = 0; i < n_par; i++) {
			population[i] = p1[i];
			if (p1[i].s.fairness < gmin_fairness) gmin_fairness = p1[i].s.fairness;
			if (p1[i].s.length < gmin_length) gmin_length = p1[i].s.length;
		}

//		finish = clock();
//		double time = (double)(finish - start) / CLOCKS_PER_SEC;
//		if (time > T) break;
	}

	finish = clock();
	double time = (double)(finish - start) / CLOCKS_PER_SEC;

	fprintf(file, "Time: %.2lf\n", time);

	fprintf(file, "%d\n", front[1].n);
	for (i = 0; i < front[1].n; i++)
		output(file, population[i].s);
	fprintf(file, "---------------------\n\n");

	sort(population, population + n_par);
	for (i = 0; i < n_par; i++)
		output(file, population[i].s);

	fclose(file);
}
