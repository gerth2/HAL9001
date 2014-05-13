#ifndef STRUCT_HEADER
#define STRUCT_HEADER

/*
***********************************************************************************************************************
***********************************************************************************************************************
*** HAL9001 Rubix Cube Solving Robot - structures.h
*** Chris Gerth - Summer 2014
*** 
*** Description: Definitions for the data structures used to represent the cube
*** Based off of http://www.cube20.org/src/cubepos.pdf
***
***********************************************************************************************************************
***********************************************************************************************************************
*/

#include <stdio.h>

#define FACE_SIZE 3
#define NUM_FACES 6
#define NUM_COLORS NUM_FACES
#define NUM_CUBIES 24


/*Define face enumeration*/
#define UP_FACE 0
#define FRONT_FACE 1
#define RIGHT_FACE 2
#define DOWN_FACE 3
#define BACK_FACE 4
#define LEFT_FACE 5
/*Properties: opposite face to i is (i+3)%NUM_FACES*/

/*Define Moves*/
#define TWIST_CW 1
#define TWIST_180_DEG 2
#define TWIST_CCW 3

/*rubiks cube object*/
typedef struct Cube_t
{
    /*corner cubies: 8 bits:  Position = [0:2], Orientation = [3:4], [5:7] = 0*/
    unsigned char c[8] = {0,0,0,0,0,0,0,0};
    /*edge cubies: 8 bits: Orientation = [0], Position = [1:4], [5:7] = 0*/
    unsigned char e[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
} Cube;















#endif