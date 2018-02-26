#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

/*
 * author: Gianna Mule, gqm2162
 * 3/1/18
 * This program simulates the spread and life of wildfires within a forest through the use of a grid with cell data to represent trees.
 * A cell may be EMPTY (0), TREE (1), BURNING (2), or BURNT (3).
 * State changes occur as a function of the variable probability, which defaults to 0.15, but may be specified by the user.
*/

int main(int argc, char ** argv) {
	int opt,size,sequence;
	float prob,treeDens,propBurn;
	char* printIts;
	
	// set defaults
	sequence = 0;
	printIts = 0;
	size = 10;
	prob = 0.15;
	treeDens = 0.15;
	propBurn = 0.15;	

	while((opt = getopt(argc,argv,"p:")) != -1) {
		switch(opt) {
			case 'p':
				sequence = 1;
				printIts = optarg;
				break;
		}
	}

	for(int i = optind; i < argc; i++) {
		printf("%s\n",argv[i]);
	}

	printf("size:%d\nprintIts:%s\nsequence:%d\nprob:%f\ntreeDens%f\npropBurn%f\n\n",size,printIts,sequence,prob,treeDens,propBurn);

return 1;

}
