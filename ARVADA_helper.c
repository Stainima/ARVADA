#include "ARVADA.h"
#include <errno.h>
#include <stdlib.h>
#include <sys/errno.h>

// Check and deal with the capicty of root node.
void check_nodes_capacity(Nodes *nodes){

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
    printf("Root-Nodes capacity increase successful: %d.\n",nodes->capacity);

}

// Check and deal with the capicty of a node.
void check_node_capacity(Node *node){

    if(node->capacity > node->num_child){
        return;
    }

    // If the number of children in the current has reached capacity, update.
    int cur_cap = node->capacity;
    node->capacity = (cur_cap + (cur_cap/2) + 1);
    void *new_space = realloc(node->children,node->capacity * sizeof(Node*));
    if(errno == ENOMEM || new_space == NULL){
        fprintf(stderr, "Error increasing the capicty of nodes.");
        return;
    }
    node->children= new_space;
    printf("Node capacity increase successful: %d.\n",node->capacity);

}
// Print the count and all trees for debugging purposes
void print_all_trees(Nodes *nodes){
    printf("capacity: %d\n",nodes->capacity);
    printf("Number of root trees: %d\n",nodes->count);

    for (int i = 0; i < nodes->count; i++){
        Node *cur = nodes->nodes[i];
        printf("Tree No.%d:",i);
        for(int j = 0; j < cur->num_child; j++){
            printf("%c",cur->children[j]->character);
        }
        printf("\n");
    }
}
