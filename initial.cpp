#include "utility.h"
#include "initial.h"
#include "dp_split.h"
#include <algorithm>
using namespace std;

extern int n;

void generate(Solution &s)
{
	int i;
	for (i = 0; i < n; i++)
		s.tour[i] = i;
	s.tour[n] = 0;
	random_shuffle(s.tour + 1, s.tour + n);

//	int p[] = {0, 21, 31, 19, 17, 13, 7, 26, 12, 1, 16, 30, 27, 24, 29, 18, 8, 9, 22,
//		15, 10, 25, 5, 20, 14, 28, 11, 4, 23, 3, 2, 6};
/*	int p[] = {0, 18, 17, 11, 40, 30, 19, 34, 44,
	29, 43, 13, 7, 28, 23,
	32, 20, 5, 21, 33, 41, 8, 10, 3,
	12, 39, 36, 42, 4, 16, 22, 9,
	26, 27, 37, 24, 6, 1,
	15, 25, 2, 38, 31, 35, 14};
*/
//	int p[] = {0, 32, 20, 5, 21, 33, 41, 8, 10, 3, 26, 27, 11, 40, 30, 19, 34, 37, 23, 28, 7, 15, 25, 2, 38, 13, 17, 18, 43, 29, 12, 39, 36, 42, 4, 16, 22, 9, 6, 44, 1, 35, 14, 24, 31 };
//	for (i = 0; i < n; i++)
//		s.tour[i] = p[i];

	if (rand() % 2) split_min_length(s);
		else split_min_fairness(s);
}
