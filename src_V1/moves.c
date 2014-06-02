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
static unsigned char corner_ori_neg_strip[NUM_CUBIES];
static unsigned char mod24[2*NUM_CUBIES];
static unsigned char corner_trans[NUM_MOVES][NUM_CUBIES];
static unsigned char edge_trans[NUM_MOVES][NUM_CUBIES];

static const unsigned char edge_twist_perm[NUM_FACES][4] = {{ 0, 2, 3, 1 },
                                                            { 3, 7, 11, 6 },
                                                            { 2, 5, 10, 7 },
                                                            { 9, 11, 10, 8 },
                                                            { 0, 4, 8, 5 },
                                                            { 1, 6, 9, 4 }};
                                                            
static const unsigned char corner_twist_perm[NUM_FACES][4] = {{ 0, 1, 3, 2 },
                                                              { 2, 3, 7, 6 },
                                                              { 3, 1, 5, 7 },
                                                              { 4, 6, 7, 5 },
                                                              { 1, 0, 4, 5 },
                                                              { 0, 2, 6, 4 }};

/*Orientations:

Edge orientation convention: 1 = flipped, 0 = not flipped.
"not flipped" means that in the home location for the cubie, the cubie is correctly oriented for the solved state.
In positions other than the home position, this changes meaning. From section 26 in http://www.cube20.org/src/cubepos.pdf,
we will define a flip as the orientation created by an R or L face CW or CCW rotation, and all other orientations do not
change edge orientation.

Corner Orientation is similar. Again, consider a cubie in the corner, and you are looking down at it toward the centre of
the rubix cube. In its home position, the orientation is either in the "solved" position, or a +/- 120 degree rotation about
an axis coming out of the corner of the cube.

Therefore, a move of the U or D face will not change the orientation. A quarter twist of any face that causes the cubie to
remain in the same face (U or D) will produce a clockwise twist on that cubie. A twist that moves a cubie from the U to the D
or from the D to the U face will produce a counter-clockwise twist.

*/                                                      
static const unsigned char edge_change[NUM_FACES] = { 0, 0, 1, 0, 0, 1 };

static const unsigned char corner_change[NUM_FACES][4] = {{ 0, 0, 0, 0 },
                                                      { 1, 2, 1, 2 },
                                                      { 1, 2, 1, 2 },
                                                      { 0, 0, 0, 0 },
                                                      { 1, 2, 1, 2 },
                                                      { 1, 2, 1, 2 }};

/*singmaster notation conversion constants*/
static unsigned char lookup_edge_cubie[NUM_FACES*NUM_FACES];
static unsigned char lookup_corner_cubie[NUM_FACES*NUM_FACES*NUM_FACES];
static unsigned char sm_corner_order[NUM_CORNERS];
static unsigned char sm_edge_order[NUM_EDGES];
static unsigned char sm_edge_flipped[NUM_EDGES];
static const char *const smedges[] = {
   "UB", "BU", "UL", "LU", "UR", "RU", "UF", "FU",
   "LB", "BL", "RB", "BR", "LF", "FL", "RF", "FR",
   "DB", "BD", "DL", "LD", "DR", "RD", "DF", "FD",
};
static const char *const smcorners[] = {
   "UBL", "URB", "ULF", "UFR", "DLB", "DBR", "DFL", "DRF",
   "LUB", "BUR", "FUL", "RUF", "BDL", "RDB", "LDF", "FDR",
   "BLU", "RBU", "LFU", "FRU", "LBD", "BRD", "FLD", "RFD",
   "ULB", "UBR", "UFL", "URF", "DBL", "DRB", "DLF", "DFR",
   "LBU", "BRU", "FLU", "RFU", "BLD", "RBD", "LFD", "FRD",
   "BUL", "RUB", "LUF", "FUR", "LDB", "BDR", "FDL", "RDF",
};
static const char * sing_solved =
"UF UR UB UL DF DR DB DL FR FL BR BL UFR URB UBL ULF DRF DFL DLB DBR" ;



/*convenience functions to deal with the cubie standards we have defined*/

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


static int corner_ori_add (int cv1, int cv2 )
{
    return mod24[cv1 + (cv2 & 0x18)];
}

static int corner_ori_sub(int cv1, int cv2 )
{
    return cv1 + corner_ori_neg_strip[cv2];
}

/*fast comparison function - returns 0 if same, not zero if different*/
static int compare_cubes(cube_t * a, cube_t * b)
{
    return memcmp(a, b, sizeof(cube_t));
}

/*internal data structure initialize functions*/
static void init_corner_ori()
{
    int i, perm, ori;
    for(i = 0; i < NUM_CUBIES; i++)
    {
        perm = corner_perm(i);
        ori = corner_ori(i);
        corner_ori_inc[i] = corner_val(perm, (ori + 1)%3);
        corner_ori_dec[i] = corner_val(perm, (ori + 2)%3);
        corner_ori_neg_strip[i] = corner_val(0, (3-ori)%3);
        mod24[i] = mod24[i+NUM_CUBIES] = i;
    }
}

static void init_identity_cube()
{
    int i;
    for(i = 0; i < NUM_CORNERS; i++)
        Identity_Cube.c[i] = corner_val(i,0);
    for(i = 0; i < NUM_EDGES; i++)
        Identity_Cube.e[i] = edge_val(i,0);

}

static void init_move_table()
{
    int m,c,perminc,isquarter,i,ii,face_iter,twist_iter,ori,new_ori;
    
    /*base initialization of translation arrays*/
    for(m = 0; m < NUM_MOVES; m++)
    {
        for(c = 0; c < NUM_CUBIES; c++)
        {
            edge_trans[m][c] = c;
            corner_trans[m][c] = c;
        }
    }
    
    /*modify translation array elements according to twists*/
    for (face_iter=0; face_iter<NUM_FACES; face_iter++)
    {
        for (twist_iter=0; twist_iter<3; twist_iter++) {
            m = face_iter * NUM_TWISTS + twist_iter;
            isquarter = (twist_iter == 0 || twist_iter == 2);
            perminc = twist_iter + 1;
           
            if (m < 0)
                continue;
                
            for (i=0; i<4; i++) 
            {
                ii = (i + perminc) % 4;
                for (ori=0; ori < 2; ori++) 
                {
                    new_ori = ori; /* new orientation*/
                    if (isquarter)
                        new_ori ^= edge_change[face_iter];
                    edge_trans[m][edge_val(edge_twist_perm[face_iter][i], ori)] = edge_val(edge_twist_perm[face_iter][ii], new_ori);
                }
                
                for (ori=0; ori <3; ori ++) 
                {
                    new_ori = ori; /* new orientation*/
                    if (isquarter)
                        new_ori = (corner_change[face_iter][i] + new_ori) % 3;
                    corner_trans[m][corner_val(corner_twist_perm[face_iter][i], ori)] = corner_val(corner_twist_perm[face_iter][ii], new_ori);
                }    
            }
       }
   }
}

static void move_cube(cube_t * in_cube, cube_t * out_cube, int move)
{
    int i;
    unsigned char * p;
    
    p = corner_trans[move];
    for(i = 0; i< NUM_CORNERS; i++)
        out_cube->c[i] = p[in_cube->c[i]];
    
    p = edge_trans[move];
     for(i = 0; i< NUM_EDGES; i++)
        out_cube->e[i] = p[in_cube->e[i]];

}

void rotate_face(cube_t * in_cube, cube_t * out_cube, unsigned char face, unsigned char direction)
{

    move_cube(in_cube, out_cube, (3*face)+(direction-1));
    return;

}

/*wrapper for all local init functions*/
void init_data_structs()
{
    init_corner_ori();
    init_identity_cube();
    init_move_table();
    initalize_Singmaster_data();
    return;
}





/*singmaster notation reading/writing functions*/

static void skip_whitespace(char *p) 
{
   while (*p && *p <= ' ')
      p++;
}

static int parse_face(char *p) 
{
   int f = decode_sigmaster_char(*p);
   if (f >= 0)
      p++;
   return f;
}

static int decode_sigmaster_char(char in) 
{
   switch (in) 
   {
        case 'u': case 'U': return 0;
        case 'f': case 'F': return 1;
        case 'r': case 'R': return 2;
        case 'd': case 'D': return 3;
        case 'b': case 'B': return 4;
        case 'l': case 'L': return 5;
        default: return -1;
   }
}

static int parse_move(char * p) 
{
   
   int twist, face;
   char *q = p;
   
   skip_whitespace(p);

   face = parse_face(q);
   if (face < 0)
      return -1;
   twist = 0;
   switch (*q) 
   {
        case '1': case '+': twist = 0; break;
        case '2': twist = 1; break;
        case '3': case '\'': case '-': twist = NUM_TWISTS-1; break;
        default: return -1;
   }
   p = q + 1;
   return face * NUM_TWISTS + twist;
}

static void append_face(char *p, int f) 
{ 
    *p++ = "UFRDBL"[f]; /*yes, hacky I know. Don't use this for securing a web server.*/
}

static void append_move(char *p, int mv) 
{
   append_face(p, mv/NUM_TWISTS);
   *p++ = "123"[mv % NUM_TWISTS];
   *p = 0;
}

static int parse_cubie(char *p) 
{
   skip_whitespace(p);
   int v = 1;
   int f = 0;
   while ((f=parse_face(p)) >= 0) {
      v = v * 6 + f;
      if (v >= 2 * 6 * 6 * 6)
         return -1;
   }
   return v;
}

static int parse_edge(char *p) 
{
   int c = parse_cubie(p);
   if (c < 6 * 6 || c >= 2 * 6 * 6)
      return -1;
   c = lookup_edge_cubie[c-6*6];
   if (c == INVALID_CUBIE)
      return -1;
   return c;
}
static int parse_corner(char *p) 
{
   int c = parse_cubie(p);
   if (c < 6 * 6 * 6 || c >= 2 * 6 * 6 * 6)
      return -1;
   c = lookup_corner_cubie[c-6*6*6];
   if (c == INVALID_CUBIE || c >= NUM_CUBIES)
      return -1;
   return c;
}

int parse_Singmaster_string(char *input, cube_t * output_cube)
{
   int m = 0;
   int c, i, cval;
   
   for (i=0; i<12; i++) {
      c = parse_edge(input) ^ sm_edge_flipped[i];
      if (c < 0)
      {
         printf("ERROR parsing Singmaster: No Such Edge %d\n", c);
         return -3;
      }
      output_cube->e[edge_perm(c)] = edge_val(sm_edge_order[i], edge_ori(c));
      m |= 1<<i;
   }
   for (i=0; i<8; i++) {
      cval = parse_corner(input);
      if (cval < 0)
      {
         printf("ERROR parsing Singmaster: No such corner %d\n", cval);
         return -4;
      }
      output_cube->c[corner_perm(cval)] = corner_ori_sub(sm_corner_order[i], cval);
      m |= 1<<(i+12);
   }
   
   skip_whitespace(input);
   if (*input)
   {
      printf("ERROR parsing Singmaster: Extra stuff after Singmaster representation\n");
      return -1;
   }
   if (m != ((1<<20)-1))
   {
      printf("ERROR parsing Singmaster: Missing at least one cubie\n");
      return -2;
   }
   return 0;
}

int create_Singmaster_string(char *output, cube_t * input_cube)
{
    unsigned char inv_corners[NUM_CORNERS];
    unsigned char inv_edges[NUM_EDGES];
    int i;
    
    /*invert so that each slot in the inv_* array has a cubie number*/
    for(i = 0; i < NUM_CORNERS; i++)
    {
        inv_corners[input_cube->c[i]] = i;
    }
    for(i = 0; i < NUM_EDGES; i++)
    {
        inv_edges[input_cube->e[i]] = i;
    }
    
    /*write cubie positions to output*/
    for(i = 0; i < NUM_CORNERS; i++)
    {
        inv_corners[input_cube->c[i]] = i;
    }
    for(i = 0; i < NUM_EDGES; i++)
    {
        inv_edges[input_cube->e[i]] = i;
    }
    

}

static void initalize_Singmaster_data()
{
    memset(lookup_edge_cubie, INVALID_CUBIE, sizeof(lookup_edge_cubie)) ;
    memset(lookup_corner_cubie, INVALID_CUBIE, sizeof(lookup_corner_cubie)) ;
    char * p;
    char * tmp;
    int i, cv;
    for (i=0; i<NUM_CUBIES; i++) 
    {
       tmp = NULL;
       lookup_corner_cubie[parse_cubie(tmp=(char *)smcorners[i])-6*6*6] = i ;
       lookup_corner_cubie[parse_cubie(tmp=(char *)smcorners[NUM_CUBIES+i])-6*6*6] = NUM_CUBIES+i ;
       lookup_edge_cubie[parse_cubie(tmp=(char *)smedges[i])-6*6] = i ;
    }
    
    p = (char *)sing_solved; //point p to the start of the solved constant string
    for (i=0; i<NUM_EDGES; i++) 
    {
       cv = parse_edge(p) ;
       sm_edge_order[i] = edge_perm(cv) ;
       sm_edge_flipped[i] = edge_ori(cv) ;
    }
    
    for (i=0; i<NUM_FACES; i++)
       sm_corner_order[i] = corner_perm(parse_corner(p)) ;

}