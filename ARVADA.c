#include "ARVADA.h"


int main (int argc, char **argv){

    // Argument check 
    if ( argc < 2){
        printf("Not enough command line arguments passed in");
    }
    
    // establishing the root node
    // Mallocing space for the node
    Node *root = malloc(sizeof(Node));


    /*
    Node sturcture for reference
    typedef struct node{
        char character;
        int order_num;
        struct node *parent;
        int num_child;
        struct node **children;
    } Node ;
    */
    
    // Initialising the root nodes.
    root->character = NULL;
    root->order_num = 0;
    root->parent = NULL;
    root->num_child = 0;
    root->children = NULL;

    // Getting the name of the file
    char *file_name = *(argv + 1);
    printf("filename = %s", file_name);
    
    // Opening the file in read mode
    FILE *file_ptr;
    file_ptr = fopen(file_name, "r");

    // Space for all the children node pointer.
    // Caping at 80 to represent good coding practices of 80 characters per line
    root->children = calloc(80, sizeof(Node*));
    
    // Reading in the file characters 1 by 1 and inttializing the root tree.
    char c;
    Node **curr_ptr;
    while((c = fgetc(file_name)) != EOF){
        
        Node *curr_node = mallco(sizeof(Node));
        curr_node->character = c;
        curr_node->parent = root;
        curr_node->order_num = root->num_child + 1;

        curr_ptr = root->children + root->num_child;
        *curr_ptr = curr_node;
        root->num_child ++;
    }

    // closing the file
    fclose(file_ptr);
    
    // freeing all thge malloced space
    while(root->num_child > 0){
        free(root->children + root->num_child - 1);
        root->num_child --;
    }
    free(root->children);
    free(root);
    return 0;
}