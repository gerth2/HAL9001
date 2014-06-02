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

#include "..\src_V1\moves.h"
#include "..\src_V1\structures.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_CUBIE_COMB 32 /*total combination of positions and orientations takes up 5 bits, so 32 possible combinations*/
#define DIST_UNDEFINED 0xFF
#define DIST_MAX 0xFE
#define NUM_HEURISTICS 5

/*structure for linked list*/
typedef struct List_Elem
{
    cube_t cube;
    unsigned char sol_dist;
    struct List_Elem * prev_elem;
    struct List_Elem * next_elem;

} listelem_t;

/*local functions*/
listelem_t * init_list();
listelem_t * append_element(cube_t * temp_cube, unsigned char sol_dist_input);
void remove_head();
void cube_error_check(cube_t * test_cube);

/*Global Variables*/
listelem_t * list_head; /*points to head node*/
listelem_t * list_tail; /*points to tail node*/

unsigned char temp_heuristic[NUM_HEURISTICS][NUM_CUBIE_COMB][NUM_CUBIE_COMB][NUM_CUBIE_COMB][NUM_CUBIE_COMB];
/*                  Corners:       0               1               3               4                7       */
/*                  Corners:       1               0               2               5                6       */
/*                    Edges:       2               1               4               7                11      */
/*                    Edges:       3               0               3               5                6       */
/*                    Edges:       4               2               8               9                10      */

/*this constant array facilitates the mapping expressed above in code:*/
const unsigned char heur_cubie_mapping[NUM_HEURISTICS][4] = {{1,3,4,7}, 
                                                             {0,2,5,6},
                                                             {1,4,7,11},
                                                             {0,3,5,6},
                                                             {2,8,9,10}};

#endif