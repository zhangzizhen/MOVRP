#include "mutation.h"
#include <algorithm>
using namespace std;

extern int n;

void mutation_operator(Solution &s)
{
	int x = rand() % (n - 1) + 1;
	int y = rand() % (n - x) + x;
	swap(s.tour[x], s.tour[y]);
}
