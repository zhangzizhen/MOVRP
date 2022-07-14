#include <cstdio>
#include <string>
#include "utility.h"
#include "read.h"
using namespace std;

extern const int MAXV;
extern const int MAXN;
extern const int MAXC;

extern int n;
extern int V;
extern int C;
extern int nc;
extern int company[MAXV];
extern int vc[MAXC];
extern double c[MAXN][MAXN];
extern double d[MAXN];
extern double reward[MAXN];
extern double cap[MAXV + 1];
extern Point p[MAXN];

void readData(string filename)
{
	printf("%s\n", filename.c_str());

	int i, j;
	FILE *file = fopen(filename.c_str(), "r");

	fscanf(file, "%d%d%d%d", &n, &V, &C, &nc);

//~~~~~~
//nc = V;
	for (i = 0; i < nc; i++)
		vc[i] = 0;

	for (i = 0; i < V; i++)
		cap[i] = C;
	cap[V] = 0x3fffffff;

	for (i = 0; i < V; i++) {
		fscanf(file, "%d", &company[i]);
//~~~~~
//company[i] = i;
		vc[company[i]]++;
	}
	company[V] = -1;

	for (i = 0; i < n; i++)
		fscanf(file, "%lf%lf", &p[i].x, &p[i].y);

	for (i = 0; i < n; i++)
	for (j = 0; j < n; j++)
		c[i][j] = int(dist(p[i], p[j]) + 0.5);
//		c[i][j] = dist(p[i], p[j]);
//		c[i][j] = dist(p[i], p[j]) * 50;
	
	for (i = 0; i < n; i++) {
		fscanf(file, "%lf", &d[i]);
		reward[i] = d[i];
//~~~
//reward[i] = 0;
//for (j = 0; j < n; j++)
//if (c[i][j] * 2 > reward[i]) reward[i] = c[i][j] * 2;
//
	}
		
	fclose(file);
}
