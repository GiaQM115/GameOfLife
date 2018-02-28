/*
 * author: Gianna Mule, gqm2162
 * 3/1/18
 * This program simulates the spread and life of wildfires within a forest through the use of a grid with cell data to represent trees.
 * A cell may be EMPTY (0), TREE (1), BURNING (2), or BURNT (3).
 * State changes occur as a function of the variable probability, which defaults to 0.15, but may be specified by the user.
*/

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "wildfire.h"

/* main simulation execution
 * @param argc: number of argument passed from command line
 * @param argv: list of arguments passed from command line (pointer)
*/
int main(int argc, char** argv) {

	// declare some variables
	static int printIterations,sequenceMode,cycles,changes,totalChanges;
	static float probability,density,proportion;
	int gridSize;	

	// handle and check arguments
	if(handleArgs(argc, argv, &gridSize, &printIterations, &sequenceMode, &probability, &density, &proportion) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}
		
	// initialize some of those variables to a default value
	cycles = 0;
	changes = 0;
	totalChanges = 0;
	if(!sequenceMode) {
		printIterations = 1;
	}

	// forest is declared here so that gridSize isn't null
	Cell forest[gridSize][gridSize];
	
	// intialize the state of the board/forest
	srand(time(NULL));	
	initBoard(gridSize,forest,density,proportion);
	
	while(cycles < printIterations) {
		
		// show this configuration and some data about this configuration
		if(sequenceMode) {
			printBoard(gridSize,forest);
		}
		else {
			//cursor controlled display method
			printIterations++; // keep the loop going forever if not in sequence mode
		}
		printf("cycle %d, size %d, probability %0.2f, density %0.2f, proportion %0.2f, changes %d\n",cycles,gridSize,probability,density,proportion,changes);	
			
		// increment total changes and set changes to 0 for next iteration
		totalChanges += changes;
		changes *= 0;
		
		// check if the latest configuration is without-fires
		if(checkFires(gridSize,forest) == EXIT_SUCCESS) {
			printf("fires are out after %d cumulative changes\n",totalChanges);
			break;
		}
	
		// configure the next board
		spread(gridSize,forest,probability,&changes);
		
		// update the board
		applySpread(gridSize,forest);
		
		// increment the number of cycles gone through
		cycles++;
	}
	
	return EXIT_SUCCESS;

}
