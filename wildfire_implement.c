/*
 * author: Gianna Mule, gqm2162
 * 3/1/18
 * This file is defines all functions for wildfire.c, where simulation takes place
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include "wildfire.h"
#define USAGE "usage: wildfire [-pN] size probability density proportion\nThe -pN option tells the simulation to print N cycles then stop\nThe probability is the probability a tree will catch on fire.\n"

/*
 * Name:	printBoard
 */
void printBoard(int size, Cell  f[][size]) {
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			printf("%c",f[i][j].symb);
		}
		printf("\n");
	}
}

/*
 * Name: 	initBoard
 */
void initBoard(int size, Cell f[][size], float dens, float prop) {
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			double d = (double)rand()/(double)RAND_MAX;
			double b = (double)rand()/(double)RAND_MAX;
			Cell c;
			if(d < dens) {
				if(b < prop) {
					c.symb = '*';
				}
				else {
					c.symb = 'Y';
				}
			}
			else {
				c.symb = ' ';
			}
			c.burnNeighbs = 0;
			f[i][j] = c;
		}
	}
	
}

/*
 * Name:	handleArgs
 * big mess of a method because passing all the params in is a pain; it's gross, it's long, I'm sorry
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
		// change integers to decimal representation of percentages
		*prob = *prob/100.0;
		*treeDens = *treeDens/100.0;
		*propBurn = *propBurn/100.0;

		return EXIT_SUCCESS;
	}
}
