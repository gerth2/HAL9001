/*
***********************************************************************************************************************
***********************************************************************************************************************
*** HAL9001 Rubix Cube Solving Robot - gen_heuristic.c
*** Chris Gerth - Summer 2014
*** 
*** Description: Creates a .h file with the heuristic lookup table in it
***
*********************************************************************************************************************** 
***********************************************************************************************************************
*/

#include "gen_heuristic.h"

int main()
{
    FILE * f =  NULL;
    cube_t temp_cube;
    unsigned char face_iter = 0;
    unsigned char twist_iter = 0;
    unsigned long num_analyzed = 0;
    int print_iter[4];
    unsigned char heur_iter = 0;
    
    init_data_structs();
    memset(temp_heuristic, DIST_UNDEFINED, sizeof(unsigned char)*32*32*32*32*5);

    /*heuristic generation will be done via a Breadth-first search implemented through a doublly linked list*/
    /*each node in the search is expressed through one element of the linked list*/
    /*the basic process is as follows: the list is initialized with the identity (solved) cube. a new element */
    /*is generated from each possible move to the cube. The number of moves to reach that configuration  */
    /*is recorded in the temporary heuristic structure and to the element itself. If a number has already been */
    /*recorded into the heuristic structure, the element is not added back into the list, as expanding its children */
    /*will not gain any new information. Once all children of an element have been attempted, it is removed from the */
    /*list, and the process is repeated on the new head of the list. The process continues until the list is empty.*/
    /*Only one process should be running at a time, so all atomicity should not matter.*/
    
    /*do all five heuristics*/
    for(heur_iter = 0; heur_iter < NUM_HEURISTICS; heur_iter ++)
    {
        
        printf("Generating Heuristic #%d\n", heur_iter);
        /*initialize linked list*/
        list_head = init_list();
        list_tail = list_head;
        num_analyzed = 0;
        /*set goal state as zero-valued heuristic*/
        if(heur_iter == 0 || heur_iter == 1) /*awkward if statement to handle e vs c */
            temp_heuristic[heur_iter][list_head->cube.c[heur_cubie_mapping[heur_iter][0]]][list_head->cube.c[heur_cubie_mapping[heur_iter][1]]][list_head->cube.c[heur_cubie_mapping[heur_iter][2]]][list_head->cube.c[heur_cubie_mapping[heur_iter][3]]] = 0;
        else
            temp_heuristic[heur_iter][list_head->cube.e[heur_cubie_mapping[heur_iter][0]]][list_head->cube.e[heur_cubie_mapping[heur_iter][1]]][list_head->cube.e[heur_cubie_mapping[heur_iter][2]]][list_head->cube.e[heur_cubie_mapping[heur_iter][3]]] = 0;
        while(list_head != NULL)
        {
            /*explore all children of the head node*/
            for(face_iter = 0; face_iter < NUM_FACES; face_iter++)
            {
                for(twist_iter = 1; twist_iter <= NUM_TWISTS; twist_iter++) /*1-based indexing for twists for mathematical simplicity*/
                {
                    /*generate a child*/
                    rotate_face(&(list_head->cube), &temp_cube, face_iter, twist_iter);
                    cube_error_check(&temp_cube); /*DEBUGGING ONLY*/
                    /*if the child is unexplored, enter distance into heuristic struct and add child to end of linked list*/
                    if(heur_iter == 0 || heur_iter == 1)
                    {
                        if(temp_heuristic[heur_iter][temp_cube.c[heur_cubie_mapping[heur_iter][0]]][temp_cube.c[heur_cubie_mapping[heur_iter][1]]][temp_cube.c[heur_cubie_mapping[heur_iter][2]]][temp_cube.c[heur_cubie_mapping[heur_iter][3]]] == DIST_UNDEFINED)
                        {
                            temp_heuristic[heur_iter][temp_cube.c[heur_cubie_mapping[heur_iter][0]]][temp_cube.c[heur_cubie_mapping[heur_iter][1]]][temp_cube.c[heur_cubie_mapping[heur_iter][2]]][temp_cube.c[heur_cubie_mapping[heur_iter][3]]]= list_head->sol_dist+1;
                            append_element(&temp_cube, list_head->sol_dist+1); /*assume uniform cost for now*/
                            num_analyzed = num_analyzed + 1;
                        }
                    }
                    else
                    {
                        if(temp_heuristic[heur_iter][temp_cube.e[heur_cubie_mapping[heur_iter][0]]][temp_cube.e[heur_cubie_mapping[heur_iter][1]]][temp_cube.e[heur_cubie_mapping[heur_iter][2]]][temp_cube.e[heur_cubie_mapping[heur_iter][3]]] == DIST_UNDEFINED)
                        {
                            temp_heuristic[heur_iter][temp_cube.e[heur_cubie_mapping[heur_iter][0]]][temp_cube.e[heur_cubie_mapping[heur_iter][1]]][temp_cube.e[heur_cubie_mapping[heur_iter][2]]][temp_cube.e[heur_cubie_mapping[heur_iter][3]]]= list_head->sol_dist+1;
                            append_element(&temp_cube, list_head->sol_dist+1); /*assume uniform cost for now*/
                            num_analyzed = num_analyzed + 1;
                        }
                    }
                }
            }
            remove_head(); /*we're done with this node, move on*/
            printf("%d\n", num_analyzed);
        }
    }
    

    /*open heuristic file for writing*/
    f=fopen("src_V1/heuristic.h", "w");
    if(f == NULL)
    {
        printf("ERROR: gen_heuristic.c - cannot open heuristic.h for writing.\n");
        return -1;
    }

    printf("Generating heuristic.h\n");
    /*write header information and guard #include's*/
    fprintf(f, " #ifndef HEURISTIC_HEADER\n");
    fprintf(f, " #define HEURISTIC_HEADER\n");
    fprintf(f, " /*\n");
    fprintf(f, " ***********************************************************************************************************************\n");
    fprintf(f, " ***********************************************************************************************************************\n");
    fprintf(f, " *** HAL9001 Rubix Cube Solving Robot - heuristic.h\n");
    fprintf(f, " *** Chris Gerth - Summer 2014\n");
    fprintf(f, " *** \n");
    fprintf(f, " *** WARNING: AUTOGENERATED. DO NOT EDIT. \n");
    fprintf(f, " *** Description: contains fixed tables for the heuristic functions\n");
    fprintf(f, " ***\n");
    fprintf(f, " ***********************************************************************************************************************\n");
    fprintf(f, " ***********************************************************************************************************************\n");
    fprintf(f, " */\n\n\n\n");
    
    fprintf(f, "unsigned char heuristic[32][32][32][32] = \n{\n");
    
    for(heur_iter = 0; heur_iter < NUM_HEURISTICS; heur_iter++)
    {
        fprintf(f, "{\n");
        fprintf(f, "/*Heuristic #%d:*/\n", heur_iter);
        for(print_iter[0] = 0; print_iter[0] < 32; print_iter[0]++)
        {
            fprintf(f, "{\n");
            for(print_iter[1] = 0; print_iter[1] < 32; print_iter[1]++)
            {
                fprintf(f, "{\n");
                fprintf(f, "/*%d, %d, X, X*/\n", print_iter[0], print_iter[1]); /*print marker*/
                for(print_iter[2] = 0; print_iter[2] < 32; print_iter[2]++)
                {
                    fprintf(f, "{");
                    for(print_iter[3] = 0; print_iter[3] < 32; print_iter[3]++)
                    {
                        if(print_iter[3] == 31)
                            fprintf(f,"%d" ,temp_heuristic[heur_iter][print_iter[0]][print_iter[1]][print_iter[2]][print_iter[3]]);
                        else
                            fprintf(f,"%d," ,temp_heuristic[heur_iter][print_iter[0]][print_iter[1]][print_iter[2]][print_iter[3]]);
                    }
                    if(print_iter[2] == 31)
                        fprintf(f, "}");
                    else
                        fprintf(f, "},");
                    fprintf(f,"\n");
                }
                if(print_iter[1] == 31)
                    fprintf(f, "}");
                else
                    fprintf(f, "},");
                fprintf(f,"\n\n\n");
            }
            if(print_iter[0] == 31)
                fprintf(f, "}");
            else
                fprintf(f, "},");
            fprintf(f,"\n\n\n\n\n");
        }
        if(heur_iter == NUM_HEURISTICS-1)
            fprintf(f, "}");
        else
            fprintf(f, "},");
        fprintf(f,"\n\n\n\n\n\n\n");
    }
    fprintf(f, "};\n\n\n\n\n\n\n\n\n\n");
    
    /*close guard includes*/
    fprintf(f, "#endif\n");
    /*close file*/
    fclose(f);
    
    
    printf("Done!\n");
    return 0;
}

/*initalizes linked list with one element*/
/*returns pointer to head of list*/
listelem_t * init_list()
{
    listelem_t * head_of_list = NULL;
    head_of_list = (listelem_t*) malloc(sizeof(listelem_t)); /*make first list element*/
    
    if(head_of_list == NULL)
    {
        printf("ERROR: Unable to allocate memory for first list element\n");
        return NULL;
    }
    
    memcpy(&(head_of_list->cube), &Identity_Cube, sizeof(cube_t)); /*set first list element's cube to be the identity (solved) cube*/
    head_of_list->prev_elem = NULL;
    head_of_list->next_elem = NULL;
    head_of_list->sol_dist = 0;
    
    return head_of_list;

}

/*makes a new element with the specified cube, and appends it to the end of the list*/
/*returns a pointer to the new element for error checking*/
listelem_t * append_element(cube_t * temp_cube, unsigned char sol_dist_input)
{
    listelem_t * new_elem = (listelem_t*) malloc(sizeof(listelem_t)); /*make new list element*/
    
    if(new_elem == NULL)
    {
        printf("ERROR: Unable to allocate memory for new list element\n");
        return NULL;
    }
    memcpy(&(new_elem->cube), temp_cube, sizeof(cube_t)); /*copy cube into new element*/
    
    new_elem->prev_elem = list_tail;
    new_elem->next_elem = NULL;
    new_elem->sol_dist = sol_dist_input;
    list_tail->next_elem = new_elem;
    
    
    list_tail = new_elem;
    
    return new_elem;


}

/*removes the head of the list, sets the old next element as the new head*/
/*frees the associated memory*/
void remove_head()
{
    listelem_t * temp;
    temp = list_head;
    list_head = list_head->next_elem;
    
    if(list_head != NULL)
        list_head->prev_elem = NULL;
        
    free(temp);
    return;
}

void cube_error_check(cube_t * test_cube)
{
    int i;
    
    for(i = 0; i < NUM_CORNERS; i++)
    {
        if(test_cube->c[i] > 31)
            printf("ERROR: corner position/orientation invalid\n");
    }
    
    for(i = 0; i < NUM_EDGES; i++)
    {
        if(test_cube->e[i] > 31)
            printf("ERROR: edge position/orientation invalid\n");
    }

}