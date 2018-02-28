/*
 * Author: Gianna Mule, gqm216
 * date: 3/1/17
 * header file for implementation of wildfire simulation
 */

#ifndef WILDFIRE_H
#define WILDFIRE_H

#define USAGE "usage: wildfire [-pN] size probability density proportion\nThe -pN option tells the simulation to print N cycles then stop\nThe probability is the probability a tree will catch on fire.\n"

/*
 * structure definition for tree struct
 * @param symb: the symbol to represent this Cell (Y, *, ' ',or _);
 * @param nextSymb: the symbol for this Cell in the next configuration (Y, *, ' ', or _)
 * @param inWindDir: holds t if this cell is in the direction of the wind, f if not
 * @param burnNeighbs: the proportion out of 100 of neighbors burning in the current configuration
 */
typedef struct Cells {
	char symb;
	char nextSymb;
	char isWindDir;
	int burnNeighbs;
} Cell;

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
 * @return EXIT_SUCCESS if all arguments are valid, else EXIT_FAILURE
 */
int handleArgs(int argc, char** argv, int* size, int* printIts, int* sequence, float* prob, float* treeDens, float* propBurn, int* speed, char** dir);

/* initialize the forrest
 2* @param size: the length of the sides of the board
 * @param f: pointer to the forrest variable
 * @param dens: the probability of a tree existing
 * @param prop: the probability of an existing tree burning
 * @return number of trees in forest
 */
int initBoard(int size, Cell f[][size], float dens, float prop);

/* print the board, cell by cell, without clearing the previous configuration
 * @param size: the width of the matrix
 * @param f: the grid/matrix/forest to be printed 
 */
void printBoard(int size, Cell f[][size]);


/*
 * collect data from this state of the board to figure out what the next configuration will look like
 * @param size: the width of the matrix
 * @param f: the grid/matrix/forest
 * @param prob: the probability of a tree catching fire
 * @param c: a pointer to the number of changes for this configuration
 */
void spread(int size, Cell f[][size], float prob, int* c, int speed, char* dir);

/*
 * the method to update the board to match the data collected from applySpread
 * @param size: the width of the matrix
 * @param f: the grid/matrix/forest
 */
void applySpread(int size, Cell f[][size]);

/* 
 * check current configuration to see if all fires have been put out
 * @param size: the width of the matrix
 * @param f: the grid/matrix/foreset
 * @return EXIT_FAILURE if there is at least 1 fire still burning, EXIT_SUCCESS else
 */
int checkFires(int size, Cell f[][size], float* ratio, int trees);

/*
`* display the board, cell by cell, using cursor controlled output
 * @param size: the width of the matrix
 * @param f: the grid/matrix/forest
 */
void writeBoard(int size, Cell f[][size]); 

#endif //end of WILDFIRE_H include guard
