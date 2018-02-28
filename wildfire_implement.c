/*
 * author: Gianna Mule, gqm2162
 * 3/1/18
 * This file is defines all functions for wildfire.c, where simulation takes place
 * associated header (wildfie.h) holds struct definition, method headers, and constants
 */

#define _BSD_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include "wildfire.h"
#include "display.h"

/*
 * determine which Cells are susceptible to increased probability of catching fire due to wind
 * @param size: the width of the matrix
 * @param f: grid/matrix/forest
 * @param speed: wind speed
 * @param dir: wind direction
 * @param row: row of current cell in question (site)
 * @param col: col of current cell in question (site)
 */
static void findWindDirs(int size, Cell f[][size], int speed, char* dir, int row, int col) {
	for(int s = 1; s < speed; s++) {
		if(strcmp(dir,"N\0")) {
			if(row - s > 0) {
				if(col + s < size && col - s > 0) {
					f[row-s][col-s].isWindDir = 't';
					f[row-s][col].isWindDir = 't';
					f[row-s][col+s].isWindDir = 't';
				}
			}
		}		
		else if(strcmp(dir,"S\0")) {
			if(row + s < size) {
				if(col + s < size && col - s > 0) {
					f[row+s][col-s].isWindDir = 't';
					f[row+s][col].isWindDir = 't';
					f[row+s][col+s].isWindDir = 't';
				}
			}
		}
		else if(strcmp(dir,"E\0")) {	
			if(col + s < size) {
				if(row + s < size && row - s > 0) {
					f[row+s][col+s].isWindDir = 't';
					f[row][col+s].isWindDir = 't';
					f[row-s][col+s].isWindDir = 't';
				}
			}
		}
		else if(strcmp(dir,"W\0")) {
			if(col - s > 0) {
				if(row + s < size && row - s > 0) {
					f[row+s][col+s].isWindDir = 't';
					f[row][col+s].isWindDir = 't';
					f[row-s][col+s].isWindDir = 't';
				}
			}
		}
	}
}

/*
 * loop through all Cells to find which neighbors are susceptible to increased threat by wind
 * @param size: the width of the matrix
 * @param f: grid/matrix/forest
 * @param speed: wind speed
 * @param dir: wind direction
 * @param row: row of current cell in question (site)
 */
static void windThreats(int size, Cell f[][size], int speed, char* dir) {
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			findWindDirs(size, f, speed, dir, i, j);
		}
	}
}

/*
 * Name:	writeBoard
 */
void writeBoard(int size, Cell f[][size]) {
	set_cur_pos(1,0);
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			put(f[i][j].symb);
		}
		set_cur_pos(i+1,0);
	}
	usleep(81000);
}

/*
 * Name:	checkFires
 */
int checkFires(int size, Cell f[][size], float* ratio, int trees) {
	int firesOut = 1;
	//find any remaining fires
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			if(f[i][j].symb == '*') {
				firesOut = 0;
				break;
			}
		}
		if(firesOut == 0) {
			break;
		}
	}
	if(firesOut == 1) {
		float b = 0;
		//count how many burned trees
		for(int i = 0; i < size; i++) {
			for(int j = 0; j < size; j++) {
				if(f[i][j].symb == '_') {
					b++;
				}
			}
		}
		*ratio = b/((float)trees);
		return EXIT_SUCCESS;
	}
	else {
		return EXIT_FAILURE;
	}
}

/*
 * Name:	applySpread
 */
void applySpread(int size, Cell f[][size]) {
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			f[i][j].symb = f[i][j].nextSymb;
		}
	}			
}

/*
 * count burning neighbors in this config for a given cell
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
 * Name:	spread
 */
void spread(int size, Cell f[][size],float prob, int* c, int speed, char* dir) {
	float probMod = 0.0;
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			windThreats(size, f, speed, dir);
			if(f[i][j].symb == 'Y') {
				countNeighbors(size, i, j, f);
				if(f[i][j].isWindDir == 't') {
					probMod = (0.5 * speed);
				}
				if((f[i][j].burnNeighbs >= 25) && ((double)rand()/(double)RAND_MAX < prob+probMod)) {
					(*c)++;
					f[i][j].nextSymb = '*';
				}
				else {
					f[i][j].nextSymb = 'Y';
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
int initBoard(int size, Cell f[][size], float dens, float prop) {
	int t = 0;
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			double d = (double)rand()/(double)RAND_MAX;
			double b = (double)rand()/(double)RAND_MAX;
			Cell c;
			if(d < dens) {
				t++;
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
	return t;
}

/*
 * Name:	handleArgs
 * big mess of a method because passing all the params in is a pain; it's gross, it's long, I'm sorry
 */
int handleArgs(int argc, char** argv, int* size, int* printIts, int* sequence, float* prob, float* treeDens, float* propBurn, int* speed, char** dir) {

	int opt;
	
	// check number of arguments
	if(argc != 5 && argc != 6 && argc != 7 && argc != 8) {
		printf(USAGE);
		return EXIT_FAILURE;
	}

	// check flag arguments (optional)
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
			default:
				fprintf(stderr, "The -pN option was invalid.\n");
				printf(USAGE);
				return EXIT_FAILURE;
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
				case 6: 
					*speed = strtol(argv[i],NULL,10);
					break;
				case 7: 
					*dir = argv[i];
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
				case 5: 
					*speed = strtol(argv[i],NULL,10);
					break;
				case 6: 
					*dir = argv[i];
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
	if(*speed < 0 || *speed > 2) {
		fprintf(stderr, "The wind speed (%d) must be an integer in [0...2].\n",*speed);
		printf(USAGE);
		return EXIT_FAILURE;
	}
	if((strcmp(*dir,"X\0") != 0) &&(strcmp(*dir,"N\0") != 0) && (strcmp(*dir,"S\0") != 0) && (strcmp(*dir,"E\0") != 0) && (strcmp(*dir,"W\0") != 0)) {
	fprintf(stderr, "The wind direction (%s) must be either N, S, E, or W.\n",*dir);
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
