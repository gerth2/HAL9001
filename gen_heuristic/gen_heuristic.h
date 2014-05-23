#ifndef GEN_HEURISTIC_HEADER
#define GEN_HEURISTIC_HEADER
/*
***********************************************************************************************************************
***********************************************************************************************************************
*** HAL9001 Rubix Cube Solving Robot - gen_heuristic.h
*** Chris Gerth - Summer 2014
*** 
*** Description: Creates a .h file with the heuristic lookup table in it
***
***********************************************************************************************************************
***********************************************************************************************************************
*/

#include "..\src\moves.h"
#include "..\src\structures.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_CUBIE_COMB 32 /*total combination of positions and orientations takes up 5 bits, so 32 possible combinations*/
#define DIST_UNDEFINED 0xFF
#define DIST_MAX 0xFE

/*structure for linked list*/
typedef struct List_Elem
{
    cube_t cube;
    unsigned char sol_dist;
    struct List_Elem * prev_elem;
    struct List_Elem * next_elem;

} listelem_t;

listelem_t * init_list();
listelem_t * append_element(cube_t * temp_cube, unsigned char sol_dist_input);
void remove_head();

/*Global Variables*/
listelem_t * list_head; /*points to head node*/
listelem_t * list_tail; /*points to tail node*/

unsigned char temp_heuristic_c1[NUM_CUBIE_COMB][NUM_CUBIE_COMB][NUM_CUBIE_COMB][NUM_CUBIE_COMB];
/*                  Corners:         1               3               4                7       */
unsigned char temp_heuristic_c2[NUM_CUBIE_COMB][NUM_CUBIE_COMB][NUM_CUBIE_COMB][NUM_CUBIE_COMB];
/*                  Corners:         2               5               6                8       */

unsigned char temp_heuristic_e1[NUM_CUBIE_COMB][NUM_CUBIE_COMB][NUM_CUBIE_COMB][NUM_CUBIE_COMB];
/*                    Edges:         1               4               7                11      */
unsigned char temp_heuristic_e2[NUM_CUBIE_COMB][NUM_CUBIE_COMB][NUM_CUBIE_COMB][NUM_CUBIE_COMB];
/*                    Edges:         3               5               6                12      */
unsigned char temp_heuristic_e3[NUM_CUBIE_COMB][NUM_CUBIE_COMB][NUM_CUBIE_COMB][NUM_CUBIE_COMB];
/*                    Edges:         2               8               9                10      */

#endif