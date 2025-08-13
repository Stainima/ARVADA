#include "ARVADA.h"
#include <errno.h>
#include <stdlib.h>
#include <sys/errno.h>

// Check and deal with the capicty of root node.
void check_nodes_capacity(Nodes *nodes){

    printf("capacity: %d\n",nodes->capacity);
    if(nodes->capacity > nodes->count){
        return;
    }

    // If number of childern has reached capacity, update cap.
    int cur_cap = nodes->capacity;
    nodes->capacity = (cur_cap + (cur_cap/2) + 1);
    void *new_space = realloc(nodes->nodes,nodes->capacity * sizeof(Node*));
    if(errno == ENOMEM || new_space == NULL){
        fprintf(stderr, "Error increasing the capicty of nodes.");
        return;
    }
    nodes->nodes = new_space;
    printf("Nodes capacity increase successful: %d.\n",nodes->capacity);

}

// Check and deal with the capicty of a node.
void check_node_capacity(Node *node){
    if(node->capacity > node->num_child){
        return;
    }

}
// Print the count and all trees for debugging purposes
void print_all_trees(Nodes *nodes){
    printf("Number of root trees: %d\n",nodes->count);
}
