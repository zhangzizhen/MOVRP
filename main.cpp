#include <stdio.h>
#include "utility.h"
#include "read.h"
#include "nsga2.h"
#include <stdlib.h>

int seed = 1;

void process(string filename, int seed)
{
	readData(filename + ".in");
	nsga2(filename + ".out" + char(seed + '0'));
}

int main(int argc, char *argv[])
{

	if (argc == 2) {
		sscanf(argv[1], "%d", &seed);
	}
	srand(seed);

	process("E051-05e", seed);

//	process("data/E051-05e", seed);
//	process("data/E076-10e", seed);
//	process("data/E101-08e", seed);
//	process("data/E101-10c", seed);
//	process("data/E121-07c", seed);
//	process("data/E151-12c", seed);
//	process("data/E200-17c", seed);

/*
	process("data/A-n32-k5", seed);
	process("data/A-n33-k5", seed);
	process("data/A-n33-k6", seed);
	process("data/A-n34-k5", seed);
	process("data/A-n36-k5", seed);
	process("data/A-n37-k5", seed);
	process("data/A-n37-k6", seed);
	process("data/A-n38-k5", seed);
	process("data/A-n39-k5", seed);
	process("data/A-n39-k6", seed);
	process("data/A-n44-k7", seed);
	process("data/A-n45-k6", seed);
	process("data/A-n45-k7", seed);
	process("data/A-n46-k7", seed);
	process("data/A-n48-k7", seed);
	process("data/A-n53-k7", seed);
	process("data/A-n54-k7", seed);
	process("data/A-n55-k9", seed);
	process("data/A-n60-k9", seed);
	process("data/A-n61-k9", seed);
	process("data/A-n62-k8", seed);
	process("data/A-n63-k10", seed);
	process("data/A-n63-k9", seed);
	process("data/A-n64-k9", seed);
	process("data/A-n65-k9", seed);
	process("data/A-n69-k9", seed);
	process("data/A-n80-k10", seed);
	process("data/B-n31-k5", seed);
	process("data/B-n34-k5", seed);
	process("data/B-n35-k5", seed);
	process("data/B-n38-k6", seed);
	process("data/B-n39-k5", seed);
	process("data/B-n41-k6", seed);
	process("data/B-n43-k6", seed);
	process("data/B-n44-k7", seed);
	process("data/B-n45-k5", seed);
	process("data/B-n45-k6", seed);
	process("data/B-n50-k7", seed);
	process("data/B-n50-k8", seed);
	process("data/B-n51-k7", seed);
	process("data/B-n52-k7", seed);
	process("data/B-n56-k7", seed);
	process("data/B-n57-k7", seed);
	process("data/B-n57-k9", seed);
	process("data/B-n63-k10", seed);
	process("data/B-n64-k9", seed);
	process("data/B-n66-k9", seed);
	process("data/B-n67-k10", seed);
	process("data/B-n68-k9", seed);
	process("data/B-n78-k10", seed);
*/
	return 0;
}
