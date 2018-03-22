# GameOfLife
Class project. Rendition of Conway's Game of Life
Code provided by professors (display.c, display.h) not altered in any way

Usage: [-pN] size prob density prop [speed] [dir]
-pN: N is the number of cycles desired. Cycles will be printed in list-format to the screen. Omitting this option will cause the simulation to run until completion, overwriting previous cycles on screen
size: An integer [5, 40] defining the size of the grid, which will be square
prob: An integer [0, 100] representing the probability of a non-burning tree catching fire, based on it's neighbors
density: An integer [0, 100] representing the percentage of the area of the grid initially accounted for by trees, burning and not
prop: An integer [0, 100] representing the percentage of the trees in the forest initially burning
speed: An integer [1, 3] representing the speed or level of the wind. Does not affect runtime performance if omitted, unless dir is provided without speed
dir: Represents the direciton of the wind if speed is provided. Options are N, S, E, W, NW, NE, SW, SE. Does not affect runtime performance if omitted, unless speed is provided without dir
