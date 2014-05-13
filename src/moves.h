
#ifndef MOVES_HEADER
#define MOVES_HEADER
/*
***********************************************************************************************************************
***********************************************************************************************************************
*** HAL9001 Rubix Cube Solving Robot - moves.h
*** Chris Gerth - Summer 2014
*** 
*** Description: Functions which manipulate cubes in computer memory
***
***********************************************************************************************************************
***********************************************************************************************************************
*/

#include <stdio.h>
#include "structures.h"


/*Local function declarations*/
static int edge_perm(int cubieval);
static int edge_ori(int cubieval);
static int corner_perm(int cubieval);
static int corner_ori(int cubieval);
static int edge_flip(int cubieval);
static int edge_val(int perm, int ori);
static int corner_val(int perm, int ori);
static int edge_ori_add (int cv1, int cv2 );
static void init_corner_ori();











#endif