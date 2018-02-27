/*
 * author: Gianna Mule, gqm2162
 * 3/1/18
 * This program simulates the spread and life of wildfires within a forest through the use of a grid with cell data to represent trees.
 * A cell may be EMPTY (0), TREE (1), BURNING (2), or BURNT (3).
 * State changes occur as a function of the variable probability, which defaults to 0.15, but may be specified by the user.
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
#include <time.h>
#define USAGE "usage: wildfire [-pN] size probability density proportion\nThe -pN option tells the simulation to print N cycles then stop\nThe probability is the probability a tree will catch on fire.\n"


/* parses arguments and checks validity; appropriately stores values if acceptable
 * @param argc: number of arguments passed into main
 * @param argv: list of arguments passed into main 
 * @param size: specified size of grid 
 * @param printIts: specified number of iterations to print to console (optional)
 * @param sequence: toggle for display method
 * @param prob: the probability a tree is going to catch on fire/burn out
 * @param treeDens: the initial probability of a tree existing
 * @param propBurn: the proportion of initial trees that are burning
 * all arguments besides argc are pointers to the variable truly holding the described data
 */
static int handleArgs(int argc, char** argv, int* size, int* printIts, int* sequence, float* prob, float* treeDens, float* propBurn) {

	// check number of arguments
	if(argc != 5 && argc != 6) {
		printf(USAGE);
		return EXIT_FAILURE;
	}

	// check flag arguments (optional)
	int opt;
	while((opt = getopt(argc,argv,"p:")) != -1) {
		switch(opt) {
			case 'p':
				*sequence = 1;
				if(isdigit(optarg[0])) {
					*printIts = strtol(optarg,NULL,10);
				}
				else {
					fprintf(stderr, "The -pN option was invalid.\n");
					printf(USAGE);
					return EXIT_FAILURE;
				}
				break;
		}
	}

	// parse and store remaining arguments
	for(int i = optind; i < argc; i++) {
		if(optind == 2) {
			switch(i) {
				case 2:
					*size = strtol(argv[i],NULL,10);
					break;
				case 3: 
					*prob = strtof(argv[i],NULL);
					break;
				case 4:
					*treeDens = strtof(argv[i],NULL);
					break;
				case 5:
					*propBurn = strtof(argv[i],NULL);
					break;
			}
		}
		else if(optind == 1) {
			switch(i) {
				case 1:
					*size = strtol(argv[i],NULL,10);
					break;	
				case 2:
					*prob = strtof(argv[i],NULL);
					break;
				case 3: 
					*treeDens = strtof(argv[i],NULL);
					break;
				case 4:
					*propBurn = strtof(argv[i],NULL);
					break;
			}
		}
	}

	// check validity of arguments and print error messages if necessary
	if(*printIts < 0) {
		fprintf(stderr, "The -pN option was negative.\n");
		printf(USAGE);
		return EXIT_FAILURE;
	}
	if(*size < 5 || *size > 40) {
		fprintf(stderr, "The size (%d) must be an integer in [5...40].\n",*size);
		printf(USAGE);
		return EXIT_FAILURE;
	}
	if(*prob < 0 || *prob > 100) {
		fprintf(stderr, "The probability (%f) must be an integer in [0...100]/\n",*prob);
		printf(USAGE);
		return EXIT_FAILURE;
	}
	if(*treeDens < 0 || *treeDens > 100) {
		fprintf(stderr, "The density (%f) must be an integer in [0...100].\n",*treeDens);
		printf(USAGE);
		return EXIT_FAILURE;
	}
	if(*propBurn < 0 || *propBurn > 100) {
		fprintf(stderr, "The proportion (%f) must be an integer in [0...100].\n",*propBurn);
		printf(USAGE);
		return EXIT_FAILURE;
	}
	else {
		// change integers to decimal representation of percentages
		*prob = *prob/100.0;
		*treeDens = *treeDens/100.0;
		*propBurn = *propBurn/100.0;

		return EXIT_SUCCESS;
	}
}

/* initialize the forrest
 * @param size: the length of the sides of the board
 * @param f: pointer to the forrest variable
 * @param dens: the probability of a tree existing
 * @param prop: the probability of an existing tree burning
 */
static void initBoard(int size, char f[][size], float dens, float prop) {
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			double d = (double)rand()/(double)RAND_MAX;
			double b = (double)rand()/(double)RAND_MAX;
			if(d < dens) {
				if(b < prop) {
					f[i][j] = '*';
				}
				else {
					f[i][j] = 'Y';
				}
			}
			else {
				f[i][j] = ' ';
			}
		}
	}
	
}

static void printBoard(int size, char f[][size]) {
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			printf("%c",f[i][j]);
		}
		printf("\n");
	}
}

/* main simulation execution
 * @param argc: number of argument passed from command line
 * @param argv: list of arguments passed from command line (pointer)
*/
int main(int argc, char** argv) {

	static int printIterations,sequenceMode,cycles,changes;
	static float probability,density,proportion;
	int gridSize;
	
	if(handleArgs(argc, argv, &gridSize, &printIterations, &sequenceMode, &probability, &density, &proportion) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	char forest[gridSize][gridSize];
	srand(time(NULL));	
	initBoard(gridSize,forest,density,proportion);

	cycles = 0;
	changes = 0;

	if(sequenceMode) {
		while(cycles < printIterations) {
			printBoard(gridSize,forest);
			printf("cycle %d, size %d, probability %0.2f, density %0.2f, proportion %0.2f, changes %d\n",cycles,gridSize,probability,density,proportion,changes);
			cycles++;
		}
	}

	return EXIT_SUCCESS;

}
