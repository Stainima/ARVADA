#include "ARVADA.h"
#include <errno.h>
#include <stdlib.h>
#include <sys/errno.h>

// Check and deal with the capicty of root node.
void check_nodes_capicity(Nodes *nodes){
    if(nodes->capacity > nodes->count){
        return;
    }

    // If number of childern has reached capicity, update cap.
    int cur_cap = nodes->capacity;
    nodes->capacity = cur_cap + (cur_cap/2) + 1;
    nodes->nodes = realloc(nodes->nodes,nodes->capacity);
    if(errno == ENOMEM){
        fprintf(stderr, "Error increasing the capicty of nodes.");
        return;
    }
    printf("Nodes capicity increase successful: %d.\n",nodes->capacity);

}

// Check and deal with the capicty of a node.
void check_node_capicity(Node *node){
    if(node->capicity > node->num_child){
        return;
    }

}
// Print the count and all trees for debugging purposes
void print_all_trees(Nodes *nodes){
    printf("Capicity: %d\n",nodes->capacity);
    printf("Number of root trees: %d\n",nodes->count);
}
