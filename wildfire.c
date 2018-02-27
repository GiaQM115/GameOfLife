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
int handleArgs(int argc, char** argv, int* size, int* printIts, int* sequence, float* prob, float* treeDens, float* propBurn) {

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
		return EXIT_SUCCESS;
	}
}

/* main simulation execution  */
int main(int argc, char** argv) {

	int gridSize,printIterations,sequenceMode;
	float probability,density,proportion;
	
	if(handleArgs(argc, argv, &gridSize, &printIterations, &sequenceMode, &probability, &density, &proportion) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;

}
