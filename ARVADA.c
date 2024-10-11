#include "ARVADA.h"

// if the node_master starts overflowing
void check_master(All_nodes* node_master){
    if(node_master->count == node_master->capicity){
        node_master->capicity = node_master->capicity + (node_master->capicity/2);
        Node **new_ptr = calloc(node_master->capicity, sizeof(Node *));
        memcpy(new_ptr, node_master->nodes, node_master->count*4);
        free(node_master->nodes);
        node_master->nodes = new_ptr;
    }
}

// This will bubble up a subtree, and return the head of the new subtree.
Node* bubble( Node *parent_node, int start, int end, All_nodes *node_master){
    
    // Ensuring a valid range
    if( start>= end || start < 0 || end > parent_node->num_child){
        printf("Invalid bubble range\n");
    }
    
    // Creating the parent bubble.
    Node *bubble_node = malloc(sizeof(Node));
    bubble_node->parent = parent_node;
    bubble_node->num_child = end - start;
    
    // adding to node master
    check_master(node_master);
    node_master->nodes[node_master->count] = bubble_node;
    node_master->count++;

    // For now, no creation of new nodes. Just creation for bubble_node. (may need to refactor)
    for (int i = start; i < end; i++){
        parent_node->children[i]->parent = bubble_node; // updating all the childrens parents
        bubble_node->children[i - start] = parent_node->children[i]; // updating the bubble node to refer to it new children.
    }
    printf("Bubbled nodes from index %d to %d\n", start, end);
    return bubble_node;
}

// If the bubble is a failure, this function will undo the bubble and revert the bubble.
void bubble_pop(Node *parent_nodem, All_nodes *node_master){

}

// Now to praise the bubble
int prase_bubble(Node *parent_node){

    return 1;
}

// if the bubble is a success, do some refactoring

int main (int argc, char **argv){

    // Argument check 
    if ( argc < 2){
        printf("Not enough command line arguments passed in");
    }
    
    // Mallocing space for all node struct
    All_nodes *node_master = malloc(sizeof(All_nodes));
    node_master->capicity = 80; // Setting initial capicity as 80
    node_master->count = 0; // Initiall no nodes, so coutn = 0
    node_master->nodes = calloc(node_master->count,sizeof(Node*)); // mallocing space for 80 nodes
    
    // establishing the root node
    // Mallocing space for the node
    Node *root = malloc(sizeof(Node));
    node_master->nodes[0] = root; // keeping track of root node
    node_master->count++; // Incrementing count

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
    root->order_num = 1;
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
    while((c = fgetc(file_ptr)) != EOF){
        
        // setting up the new node
        Node *curr_node = malloc(sizeof(Node));
        curr_node->character = c;
        curr_node->parent = root;
        curr_node->order_num = root->num_child + 1;

        // adding it to node_master
        node_master->nodes[node_master->count] = curr_node;
        node_master->count++;
        check_master(node_master);
        root->num_child ++;

        // adding to the children of the root node
        root->children[root->num_child] = curr_node;
        root->num_child++;
    }

    // closing the file
    fclose(file_ptr);
    
    // freeing all thge malloced space
    for(int i = 0; i < node_master->count; i++){
        if(node_master->nodes[i]->children){
            free(node_master->nodes[i]->children);
        }
        free(node_master->nodes[i]); 
    }
    free(node_master->nodes);
    free(node_master);
    return 0;
}