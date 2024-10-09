#include "ARVADA.h"

// This will bubble up a subtree, and return the head of the new subtree.
Node* bubble( Node *parent_node, int start, int end){
    
    // Ensuring a valid range
    if( start>= end || start < 0 || end > parent_node->num_child){
        printf("Invalid bubble range\n");
    }
    
    // Creating the parent bubble.
    Node *bubble_node = malloc(sizeof(Node));
    bubble_node->parent = parent_node;
    bubble_node->num_child = end - start;

    // For now, no creation of new nodes. Just creation for bubble_node. (mane need to refactor)
    for (int i = start; i < end; i++){
        parent_node->children[i]->parent = bubble_node; // updating all the childrens parents
        bubble_node->children[i - start] = parent_node->children[i]; // updating the bubble node to refer to it new children.
    }

}

// If the bubble is a failure, this function will undo the bubble and revert the bubble.
void bubble_pop(Node *parent_node){

}

// 
int prase_bubble(Node *parent_node){

}

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
    root->children = calloc(80, sizeof(Node*)); // note: may need to be changed later. (memcpy)
    
    // Reading in the file characters 1 by 1 and inttializing the root tree.
    char c;
    Node **curr_ptr;
    while((c = fgetc(file_ptr)) != EOF){
        
        Node *curr_node = malloc(sizeof(Node));
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