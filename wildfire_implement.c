/*
 * author: Gianna Mule, gqm2162
 * 3/1/18
 * This file is defines all functions for wildfire.c, where simulation takes place
 * associated header (wildfie.h) holds struct definition, method headers, and constants
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include "wildfire.h"

/*
 * Name:	spread
 */
void spread(int size, Cell f[][size]) {
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			f[i][j].symb = f[i][j].nextSymb;
		}
	}			
}

/*
 * Name:	countNeighbors
 * private method to count burning neighbors in this config for a given cell
 * @param size: size of matrix
 * @param row: row of cell in question
 * @param col: column of cell in question
 * @param f: grid/matrix/forest
 */
static void countNeighbors(int size, int row, int col, Cell f[][size]) {
	int rowBefore, rowAfter, colBefore, colAfter;
	float burning,total;
	rowBefore = row-1;
	rowAfter = row+1;
	colBefore = col-1;
	colAfter = col+1;
	burning = 0;
	total = 0;

	// check all possible spots for neighbors
	if(colBefore > 0) {
		total++;
		if(f[row][colBefore].symb == '*') {
			burning++;
		}
	}

	if(colAfter < size) {
		total++;
		if(f[row][colAfter].symb == '*') {
			burning++;
		}
	}

	if(rowBefore > 0) {
		if(colBefore > 0) {
			total++;
			if(f[rowBefore][colBefore].symb == '*') {
				burning++;
			}
		}
		total++;
		if(f[rowBefore][col].symb == '*') {
			burning++;
		}
		if(colAfter < size) {
			total++;
			if(f[rowBefore][colAfter].symb == '*') {
				burning++;
			}
		}
	}

	if(rowAfter < size) {
		if(colBefore > 0) {
			total++;
			if(f[rowAfter][colBefore].symb == '*') {
				burning++;
			}
		}
		total++;
		if(f[rowAfter][col].symb == '*') {
			burning++;
		}
		if(colAfter < size) {
			total++;
			if(f[rowAfter][colAfter].symb == '*') {
				burning++;
			}
		}
	}

	// calculate proportion burning
	float b = burning/total;
	b = b*100;
	f[row][col].burnNeighbs = ((int)b);
}


/*
 * Name:	applySpread
 */
void applySpread(int size, Cell f[][size],float prob, int* c) {
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			if(f[i][j].symb == 'Y') {
				countNeighbors(size, i, j, f);
				if((f[i][j].burnNeighbs >= 25) && ((double)rand()/(double)RAND_MAX < prob)) {
					(*c)++;
					f[i][j].nextSymb = '*';
				}
			}
			else if((f[i][j].symb == '*') && (double)rand()/(double)RAND_MAX < prob) {
					(*c)++;					
					f[i][j].nextSymb = '_';
			}
			else {
				f[i][j].nextSymb = f[i][j].symb;
			}
		}
	}		
}

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
