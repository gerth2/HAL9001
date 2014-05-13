
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
#include <string.h>
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
static int corner_ori_add (int cv1, int cv2 );
static int corner_ori_sub(int cv1, int cv2 );
static int compare_cubes(cube_t * a, cube_t * b);
static void init_corner_ori();
static void init_identity_cube();
static void init_move_table();
static void move_cube(cube_t * in_cube, cube_t * out_cube, int move);

static void skip_whitespace(char * p);
static int parse_face(char * p);
static int decode_sigmaster_char(char in);
static int parse_move(char * p);
static void append_face(char * p, int f);
static void append_move(char * p, int mv);
static int parse_cubie(char * p);
static int parse_edge(char * p);
static int parse_corner(char * p);
static int parse_Singmaster(char *input, cube_t * output_cube);
static void initalize_Singmaster_data();


/*externally available functions*/
void init_data_structs();
int rotate_face(cube_t * in_cube, cube_t * out_cube, unsigned char face, unsigned char direction);
int create_Singmaster_string(char *output, cube_t * input_cube);
int parse_Singmaster_string(char *input, cube_t * output_cube);


extern cube_t Identity_Cube;











#endif