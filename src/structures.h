#ifndef STRUCT_HEADER
#define STRUCT_HEADER

/*
***********************************************************************************************************************
***********************************************************************************************************************
*** HAL9001 Rubix Cube Solving Robot - structures.h
*** Chris Gerth - Summer 2014
*** 
*** Description: Definitions for the data structures used to represent the cube
***
***********************************************************************************************************************
***********************************************************************************************************************
*/

#include <stdio.h>

#define FACE_SIZE 3
#define NUM_FACES 6
#define NUM_COLORS NUM_FACES

#define YELLOW 0
#define GREEN 1
#define ORANGE 2
#define BLUE 3
#define WHITE 4
#define RED 5

/* Solved Configuration (rolled out): 

      4 4 4
      4 4 4
      4 4 4
0 0 0 1 1 1 2 2 2 3 3 3
0 0 0 1 1 1 2 2 2 3 3 3
0 0 0 1 1 1 2 2 2 3 3 3
      5 5 5
      5 5 5
      5 5 5

*/

/*Cube = 6x3x3 array of colors*/
/*ex: unsigned char cube [NUM_FACES][FACE_SIZE][FACE_SIZE];*/
















#endif