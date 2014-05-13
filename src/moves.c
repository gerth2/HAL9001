/*
***********************************************************************************************************************
***********************************************************************************************************************
*** HAL9001 Rubix Cube Solving Robot - moves.c
*** Chris Gerth - Summer 2014
*** 
*** Description: Functions which manipulate cubes in computer memory
***
***********************************************************************************************************************
***********************************************************************************************************************
*/

#include "moves.h"

static unsigned char corner_ori_inc[NUM_CUBIES];
static unsigned char corner_ori_dec[NUM_CUBIES];
static unsigned char corner_ori_neg_stri[CUBIES];
static unsigned char mod24[2*CUBIES];


int rotate_face(Cube * in_cube, t_cube out_cube, unsigned char face, unsigned char direction)
{
    printf("Running move.c");
    return 0;

}

static int edge_perm(int cubieval)
{
    return cubieval >> 1;
}

static int edge_ori(int cubieval)
{
    return cubieval & 1;
}

static int corner_perm(int cubieval)
{
    return cubieval & 7;
}

static int corner_ori(int cubieval)
{
    return cubieval >> 3;
}

static int edge_flip(int cubieval)
{
    return cubieval ^ 1;
}

static int edge_val(int perm, int ori)
{
    return perm*2 + ori;
}

static int corner_val(int perm, int ori)
{
    return ori*8 + perm;
}

static int edge_ori_add (int cv1, int cv2 )
{
    return cv1 ^ edge_ori(cv2 );
}

/*
static int corner_ori_add (int cv1 ; int cv2 )
{
    return mod24[cv1 + (cv2 & #18)];
}

static int corner_ori_sub(int cv1 ; int cv2 )
{
    return cv1 + corner ori neg strip[cv2 ];
}
*/

static int compare_cubies(unsigned char * a, unsigned char * b)

static void init_corner_ori()
{
    int i, perm, ori;
    for(i = 0; i < CUBIES; i++)
    {
        perm = corner_perm(i);
        ori = corner_ori(i);
        corner_ori_inc[i] = corner_val(perm, (ori + 1)%3);
        corner_ori_dec[i] = corner_val(perm, (ori + 2)%3);
        corner_ori_neg_strip[i] = corner_val(0, (3-ori)%3);
        mod24[i] = mod24[i+CUBIES] = i;
    }
}