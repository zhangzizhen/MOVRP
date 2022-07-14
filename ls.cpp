#include "utility.h"
#include "ls.h"
#include <cstdlib>
#include <cmath>
using namespace std;

extern int n;
extern int V;
extern int C;

extern double alpha;
extern double beta;

extern double d[MAXN];
extern double cap[MAXV + 1];
extern double c[MAXN][MAXN];
extern double reward[MAXN];

// exchange two customers
void ls_exchange(Solution &s, double weight[])
{
	int r1, r2;
	if (s.r[V].nv > 1) {
		r1 = rand() % (V + 1);
		r2 = rand() % (V + 1);
	} else {
		r1 = rand() % V;
		r2 = rand() % V;
	}
	if (s.r[r1].nv > 1 && s.r[r2].nv > 1) {
		int k1 = rand() % (s.r[r1].nv - 1) + 1;
		int k2 = rand() % (s.r[r2].nv - 1) + 1;
		int x = s.r[r1].v[k1];
		int y = s.r[r2].v[k2];
		if (x == y) return;
		if (r1 == r2 ||
			(s.r[r1].load - d[x] + d[y] <= cap[r1] + 1e-5 &&
			s.r[r2].load - d[y] + d[x] <= cap[r2] + 1e-5)) {
			double dt1, dt2;
			if (r1 == r2 && k2 == k1 + 1) {
				dt1 = c[s.r[r1].v[k1 - 1]][y] - c[s.r[r1].v[k1 - 1]][x];
				dt2 = c[x][s.r[r2].v[k2 + 1]] - c[y][s.r[r2].v[k2 + 1]];
			} else
			if (r1 == r2 && k1 == k2 + 1) {
				 dt1 = c[s.r[r2].v[k2 - 1]][x] - c[s.r[r2].v[k2 - 1]][y];
				 dt2 = c[y][s.r[r1].v[k1 + 1]] - c[x][s.r[r1].v[k1 + 1]];
			} else {
				dt1 = c[s.r[r1].v[k1 - 1]][y] + c[y][s.r[r1].v[k1 + 1]] - c[s.r[r1].v[k1 - 1]][x] - c[x][s.r[r1].v[k1 + 1]];
				dt2 = c[s.r[r2].v[k2 - 1]][x] + c[x][s.r[r2].v[k2 + 1]] - c[s.r[r2].v[k2 - 1]][y] - c[y][s.r[r2].v[k2 + 1]];
			}

			Solution ss;
			for (int i = 0; i <= V; i++) {
				ss.r[i].load = s.r[i].load;
				ss.r[i].length = s.r[i].length;
				ss.r[i].profit = s.r[i].profit;
			}
			ss.r[r1].load += d[y] - d[x];
			ss.r[r2].load += d[x] - d[y];
			ss.r[r1].length += dt1;
			ss.r[r2].length += dt2;
			ss.r[r1].profit += alpha * (reward[y] - reward[x]) - beta * dt1;
			ss.r[r2].profit += alpha * (reward[x] - reward[y]) - beta * dt2;
			calculate_solution_length(ss);
			calculate_solution_fairness(ss);

			if (weight[0] * ss.length + weight[1] * ss.fairness < weight[0] * s.length + weight[1] * s.fairness) {
				// update
				s.length = ss.length;
				s.fairness = ss.fairness;
				s.r[r1].load = ss.r[r1].load;
				s.r[r2].load = ss.r[r2].load;
				s.r[r1].length = ss.r[r1].length;
				s.r[r2].length = ss.r[r2].length;
				s.r[r1].profit = ss.r[r1].profit;
				s.r[r2].profit = ss.r[r2].profit;
				s.r[r1].v[k1] = y;
				s.r[r2].v[k2] = x;
			}
		}
	}
}

// insert a customer to the position before another customer
void ls_insert(Solution &s, double weight[])
{
	int r1, r2;
	if (s.r[V].nv > 1) r1 = rand () % (V + 1);
		else r1 = rand() % V;
	r2 = rand() % V;

	if (s.r[r1].nv > 1 && s.r[r2].nv > 1) {
		int k1 = rand() % (s.r[r1].nv - 1) + 1;
		int k2 = rand() % s.r[r2].nv + 1;
		int x = s.r[r1].v[k1];
		int y = s.r[r2].v[k2];
		if (x == y || (r1 == r2 && k1 + 1 == k2)) return;

		if (r1 == r2 || s.r[r2].load + d[x] <= cap[r2]) {
			double dt1, dt2;
			dt1 = c[s.r[r1].v[k1 - 1]][s.r[r1].v[k1 + 1]] - c[s.r[r1].v[k1 - 1]][x] - c[x][s.r[r1].v[k1 + 1]];
			dt2 = c[s.r[r2].v[k2 - 1]][x] + c[x][y] - c[s.r[r2].v[k2 - 1]][y];

			Solution ss;
			for (int i = 0; i <= V; i++) {
				ss.r[i].load = s.r[i].load;
				ss.r[i].length = s.r[i].length;
				ss.r[i].profit = s.r[i].profit;
			}
			ss.r[r1].load += -d[x];
			ss.r[r2].load += d[x];
			ss.r[r1].length += dt1;
			ss.r[r2].length += dt2;
			ss.r[r1].profit += -alpha * reward[x] - beta * dt1;
			ss.r[r2].profit += alpha * reward[x] - beta * dt2;
			calculate_solution_length(ss);
			calculate_solution_fairness(ss);

			if (weight[0] * ss.length + weight[1] * ss.fairness < weight[0] * s.length + weight[1] * s.fairness) {
				// update
				s.length = ss.length;
				s.fairness = ss.fairness;
				s.r[r1].load = ss.r[r1].load;
				s.r[r2].load = ss.r[r2].load;
				s.r[r1].length = ss.r[r1].length;
				s.r[r2].length = ss.r[r2].length;
				s.r[r1].profit = ss.r[r1].profit;
				s.r[r2].profit = ss.r[r2].profit;
				for (int j = k1; j < s.r[r1].nv; j++)
					s.r[r1].v[j] = s.r[r1].v[j + 1];
				s.r[r1].nv--;
				// same route
				if (r1 == r2 && k2 > k1) k2--;
				for (int j = s.r[r2].nv; j >= k2; j--)
					s.r[r2].v[j + 1] = s.r[r2].v[j];
				s.r[r2].v[k2] = x;
				s.r[r2].nv++;

				s.r[r1].v[s.r[r1].nv] = 0;
				s.r[r2].v[s.r[r2].nv] = 0;
			}
		}
	}
}

void ls_2opt(Solution &s, double weight[])
{
	int r1, r2;
	if (s.r[V].nv > 1) {
		r1 = rand() % (V + 1);
		r2 = rand() % (V + 1);
	} else {
		r1 = rand() % V;
		r2 = rand() % V;
	}
	if (r1 != r2 && s.r[r1].nv > 1 && s.r[r2].nv > 1) {
		int k1 = rand() % (s.r[r1].nv - 1) + 1;
		int k2 = rand() % (s.r[r2].nv - 1) + 1;
		Solution ss;
		int i;
		for (i = 0; i <= V; i++) {
			ss.r[i].load = s.r[i].load;
			ss.r[i].length = s.r[i].length;
			ss.r[i].profit = s.r[i].profit;
		}
		for (i = 0; i < k1; i++)
			ss.r[r1].v[i] = s.r[r1].v[i];
		for (i = 0; i < k2; i++)
			ss.r[r2].v[i] = s.r[r2].v[i];
		ss.r[r1].nv = k1;
		for (i = k2; i < s.r[r2].nv; i++)
			ss.r[r1].v[ss.r[r1].nv++] = s.r[r2].v[i];
		ss.r[r2].nv = k2;
		for (i = k1; i < s.r[r1].nv; i++)
			ss.r[r2].v[ss.r[r2].nv++] = s.r[r1].v[i];
		calculate_route_load(ss.r[r1]);
		calculate_route_load(ss.r[r2]);
		if (ss.r[r1].load <= cap[r1] + 1e-5 && ss.r[r2].load <= cap[r2] + 1e-5) {
			calculate_route_length(ss.r[r1]);
			calculate_route_length(ss.r[r2]);
			calculate_route_profit(ss.r[r1]);
			calculate_route_profit(ss.r[r2]);
			calculate_solution_length(ss);
			calculate_solution_fairness(ss);
			if (weight[0] * ss.length + weight[1] * ss.fairness < weight[0] * s.length + weight[1] * s.fairness) {
				// update
				s.length = ss.length;
				s.fairness = ss.fairness;
				s.r[r1].load = ss.r[r1].load;
				s.r[r2].load = ss.r[r2].load;
				s.r[r1].length = ss.r[r1].length;
				s.r[r2].length = ss.r[r2].length;
				s.r[r1].profit = ss.r[r1].profit;
				s.r[r2].profit = ss.r[r2].profit;
				for (i = 0; i < ss.r[r1].nv; i++)
					s.r[r1].v[i] = ss.r[r1].v[i];
				s.r[r1].v[i] = 0;
				s.r[r1].nv = ss.r[r1].nv;
				for (i = 0; i < ss.r[r2].nv; i++)
					s.r[r2].v[i] = ss.r[r2].v[i];
				s.r[r2].v[i] = 0;
				s.r[r2].nv = ss.r[r2].nv;
			}
		}
	}
}

void ls_reverse(Solution &s, double weight[])
{
	int r = rand() % V;
	if (s.r[r].nv > 2) {
		int k1 = rand() % (s.r[r].nv - 2) + 1;
		int k2 = rand() % (s.r[r].nv - 1 - k1) + k1 + 1;
		Solution ss;
		for (int i = 0; i <= V; i++) {
			ss.r[i].length = s.r[i].length;
			ss.r[i].profit = s.r[i].profit;
		}
		double dt = - c[s.r[r].v[k1 - 1]][s.r[r].v[k1]] + c[s.r[r].v[k1 - 1]][s.r[r].v[k2]]
		            - c[s.r[r].v[k2]][s.r[r].v[k2 + 1]] + c[s.r[r].v[k1]][s.r[r].v[k2 + 1]];
		ss.r[r].length += dt;
		ss.r[r].profit -= beta * dt;
		calculate_solution_length(ss);
		calculate_solution_fairness(ss);

		if (weight[0] * ss.length + weight[1] * ss.fairness < weight[0] * s.length + weight[1] * s.fairness) {
			s.length = ss.length;
			s.fairness = ss.fairness;
			s.r[r].length = ss.r[r].length;
			s.r[r].profit = ss.r[r].profit;
			for (int i = k1, j = k2; i < j; i++, j--) {
				int k = s.r[r].v[i];
				s.r[r].v[i] = s.r[r].v[j];
				s.r[r].v[j] = k;
			}
		}
	}
}

bool check(Solution &s)
{
	for (int i = 0; i <= V; i++) {
		double len = s.r[i].length;
		double pro = s.r[i].profit;
		double loa = s.r[i].load;
		calculate_route_load(s.r[i]);
		calculate_route_length(s.r[i]);
		calculate_route_profit(s.r[i]);
		if (fabs(s.r[i].length - len) > 1e-5 || fabs(s.r[i].load - loa) > 1e-5 || fabs(s.r[i].profit - pro) > 1e-5) return false;
	}
	return true;
}

void local_search_operator(Solution &s, double weight[], int ls_iter)
{
	for (int i = 0; i < ls_iter; i++) {
		int r = rand() % 4;
		switch (r) {
		case 0:
			ls_exchange(s, weight);
			break;
		case 1:
			ls_insert(s, weight);
			break;
		case 2:
			ls_2opt(s, weight);
			break;
		case 3:
			ls_reverse(s, weight);
			break;
		}
//		if (!check(s)) output(s);

	}

	make_giant_tour(s);

}
