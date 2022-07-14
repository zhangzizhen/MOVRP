#include "recombination.h"
#include <cstdlib>
using namespace std;

extern int n;

void recombination_operator(const Solution &p1, const Solution &p2, Solution &o1, Solution &o2)
{
	int i, j;
	int a[n];
	int x = rand() % n;
	int y = rand() % (n - x) + x + 1;

	// (1..n) X (1..n)

	// PMX for offspring 1
	for (i = 0; i < n; i++)
		a[i] = i;
	for (i = x; i < y; i++)
		a[p2.tour[i]] = p1.tour[i];
	for (i = 0; i < n; i++) {
		while (a[a[i]] != a[i]) {
			j = a[a[i]];
			a[a[i]] = a[i];
			a[i] = j;
		}
	}
	for (i = 0; i < x; i++)
		o1.tour[i] = a[p1.tour[i]];
	for (i = x; i < y; i++)
		o1.tour[i] = p2.tour[i];
	for (i = y; i < n; i++)
		o1.tour[i] = a[p1.tour[i]];

	// PMX for offspring 2
	for (i = 0; i < n; i++)
		a[i] = i;
	for (i = x; i < y; i++)
		a[p1.tour[i]] = p2.tour[i];
	for (i = 0; i < n; i++) {
		while (a[a[i]] != a[i]) {
			j = a[a[i]];
			a[a[i]] = a[i];
			a[i] = j;
		}
	}
	for (i = 0; i < x; i++)
		o2.tour[i] = a[p2.tour[i]];
	for (i = x; i < y; i++)
		o2.tour[i] = p1.tour[i];
	for (i = y; i < n; i++)
		o2.tour[i] = a[p2.tour[i]];
}
