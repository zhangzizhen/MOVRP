#include "dp_split.h"
#include "utility.h"

extern int n;
extern int V;
extern int C;
extern double alpha;
extern double beta;

extern double c[MAXN][MAXN];
extern double d[MAXN];
extern double reward[MAXN];


const double inf = 1e10;

void split_min_length(Solution &s)
{
	int i, j, k, t;
	double a[V + 1][n];
	int link[V + 1][n];
	a[0][0] = 0;
	link[0][0] = -1;
	for (i = 1; i < n; i++) {
		a[0][i] = -inf;
		link[0][i] = -1;
	}

	for (j = 0; j < V; j++) {
		for (i = 0; i < n; i++) {
			a[j + 1][i] = -inf;
			link[j + 1][i] = -1;
		}
		for (i = 0; i < n; i++)
		if (a[j][i] > -inf) {
			double cap = 0;
			double len = 0;
			int last = 0;
			for (k = i + 1; k < n; k++)
			if (cap + d[s.tour[k]] <= C + 1e-5) {
				cap += d[s.tour[k]];
				len += c[last][s.tour[k]];
				last = s.tour[k];
				double tmp = a[j][i] + (len + c[last][0]);
				if (a[j + 1][k] <= -inf || tmp < a[j + 1][k]) {
					a[j + 1][k] = tmp;
					link[j + 1][k] = i;
				}
			}
			else break;
		}			
	}
	
//	printf("%lf\n", a[V][n - 1]);

	for (j = 0; j <= V; j++) {
		s.r[j].v[0] = 0;
		s.r[j].nv = 1;
	}
	
	t = n - 1;
	while (a[V][t] <= -inf) t--;

	i = t;
	j = V;
	while (link[j][i] >= 0) {
		for (k = link[j][i] + 1; k <= i; k++)
			s.r[j - 1].v[s.r[j - 1].nv++] = s.tour[k];
		s.r[j - 1].v[s.r[j - 1].nv] = 0;
		i = link[j][i];
		j--;
	}

	for (i = t + 1; i < n; i++)
		s.r[V].v[s.r[V].nv++] = s.tour[i];
	s.r[V].v[s.r[V].nv] = 0;

	for (j = 0; j <= V; j++) {
		calculate_route_load(s.r[j]);
		calculate_route_length(s.r[j]);
		calculate_route_profit(s.r[j]);
	}
	calculate_solution_length(s);
	calculate_solution_fairness(s);

	// in case of infeasible solution
//	s.length += (n - 1 - t) * 100000;
//	s.fairness += (n - 1 - t) * 100000;
}

void split_min_f(Solution &s)
{




}



void split_min_fairness(Solution &s)
{
	int i, j, k, t;
	double a[MAXV + 1][MAXN + 1];
	int link[MAXV + 1][MAXN + 1];
	a[0][0] = 0;
	link[0][0] = -1;
	for (i = 1; i < n; i++) {
		a[0][i] = -inf;
		link[0][i] = -1;
	}

	for (j = 0; j < V; j++) {
		for (i = 0; i < n; i++) {
			a[j + 1][i] = -inf;
			link[j + 1][i] = -1;
		}
		for (i = 0; i < n; i++)
		if (a[j][i] > -inf) {
			double cap = 0;
			double len = 0;
			double rew = 0;
			int last = 0;
			for (k = i + 1; k < n; k++)
			if (cap + d[s.tour[k]] <= C + 1e-5) {
				cap += d[s.tour[k]];
				rew += reward[s.tour[k]];
				len += c[last][s.tour[k]];
				last = s.tour[k];
				double tmp = alpha * rew - beta * (len + c[last][0]);
				if (a[j][i] > tmp) tmp = a[j][i];
				if (a[j + 1][k] <= -inf || tmp < a[j + 1][k]) {
					a[j + 1][k] = tmp;
					link[j + 1][k] = i;
				}
			}
			else break;
		}
	}
	
//	printf("%lf\n", a[V][n - 1]);

	for (j = 0; j <= V; j++) {
		s.r[j].v[0] = 0;
		s.r[j].nv = 1;
	}
	
	t = n - 1;
	while (a[V][t] <= -inf) t--;

	i = t;
	j = V;
	while (link[j][i] >= 0) {
		for (k = link[j][i] + 1; k <= i; k++)
			s.r[j - 1].v[s.r[j - 1].nv++] = s.tour[k];
		s.r[j - 1].v[s.r[j - 1].nv] = 0;
		i = link[j][i];
		j--;
	}

	for (i = t + 1; i < n; i++)
		s.r[V].v[s.r[V].nv++] = s.tour[i];
	s.r[V].v[s.r[V].nv] = 0;

	for (j = 0; j <= V; j++) {
		calculate_route_load(s.r[j]);
		calculate_route_length(s.r[j]);
		calculate_route_profit(s.r[j]);
	}
	calculate_solution_length(s);
	calculate_solution_fairness(s);

	// in case of infeasible solution
//	s.length += (n - 1 - t) * 100000;
//	s.fairness += (n - 1 - t) * 100000;
}

