#include "lns.h"
#include "utility.h"
#include <algorithm>
using namespace std;

extern int n;
extern int V;
extern int C;

extern double c[MAXN][MAXN];
extern double d[MAXN];
extern double reward[MAXN];

void LNS_operator(Solution &s)
{
	int i, j, k;

	int collect[n];
	int nc = 0;

	Solution ss;
	// ruin process
	for (i = 0; i <= V; i++) {
		ss.r[i].nv = 1;
		ss.r[i].v[0] = 0;
		for (j = 1; j < s.r[i].nv; j++) {
			if (random(0,1) > 0.1) // greater than removal rate
				ss.r[i].v[ss.r[i].nv++] = s.r[i].v[j];
			else collect[nc++] = s.r[i].v[j];
		}
		ss.r[i].v[ss.r[i].nv] = 0;
		calculate_route_load(ss.r[i]);
		calculate_route_length(ss.r[i]);
		calculate_route_profit(ss.r[i]);
	}

	// recreate process
	random_shuffle(collect, collect + nc);
	for (i = 0; i < nc; i++) {
		double minr = 1e10;
		int jj, kk;
		for (j = 0; j < V; j++)
		if (ss.r[j].load + d[collect[i]] <= C + 1e-5) {
			for (k = 0; k < ss.r[j].nv; k++) {
				double regret = c[ss.r[j].v[k]][collect[i]] + c[collect[i]][ss.r[j].v[k + 1]]
					- c[ss.r[j].v[k]][ss.r[j].v[k + 1]];
				if (regret < minr) {
					minr = regret;
					jj = j;
					kk = k;
				}
			}
		}
		if (minr < 1e10) {
			for (k = ss.r[jj].nv; k > kk; k--)
				ss.r[jj].v[k + 1] = ss.r[jj].v[k];
			ss.r[jj].v[kk + 1] = collect[i];
			ss.r[jj].nv++;
			calculate_route_load(ss.r[jj]);
			//		ss.r[jj].load += d[collect[i]];
			calculate_route_length(ss.r[jj]);
			//		ss.r[jj].length += minr;
			calculate_route_profit(ss.r[jj]);
		} else {
			// cannot assign
			ss.r[V].v[ss.r[V].nv++] = collect[i];
			ss.r[V].v[ss.r[V].nv] = 0;
			calculate_route_load(ss.r[V]);
			calculate_route_length(ss.r[V]);
			calculate_route_profit(ss.r[V]);
		}
	}

	calculate_solution_length(ss);
	calculate_solution_fairness(ss);

	if (is_dominate(ss, s)) {
		s = ss;
		make_giant_tour(s);
	}
}

void LNS(Solution &s)
{
	int i;
	for (i = 0; i < 100; i++)
		LNS_operator(s);
}
