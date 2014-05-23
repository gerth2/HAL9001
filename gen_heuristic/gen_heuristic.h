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

/*structure for linked list*/
typedef struct List_Elem
{
    cube_t cube;
    struct List_Elem * prev_elem;
    struct List_Elem * next_elem;

} listelem_t;

listelem_t * init_list();
listelem_t * append_element(cube_t * temp_cube);
void remove_head();

#endif