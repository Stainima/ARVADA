#include "ARVADA.h"
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/errno.h>

// ------------- Funcitons to help build the original naive trees
// Section III-A of original paper

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
    //printf("Root-Nodes capacity increase successful: %d.\n",nodes->capacity);

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
    //printf("Node capacity increase successful: %d.\n",node->capacity);

}

// Print the count and all trees for debugging purposes
void print_all_trees(Nodes *nodes){
    //printf("capacity: %d\n",nodes->capacity);
    //printf("Number of root trees: %d\n",nodes->count);

    for (int i = 0; i < nodes->count; i++){
        Node *cur = nodes->nodes[i];
        printf("Tree No.%d:",i);
        for(int j = 0; j < cur->num_child; j++){
            printf("%c",cur->children[j]->character);
        }
        printf("\n");
    }
}
//---------------------------------------

// Function to concatenate the nodes into a string given a root node
void concatenate(Node *root, char** buffer){

    if(root->t == -1){
        size_t len = strlen(*buffer);
        char *new_space = realloc(*buffer, (len + 2) * sizeof(char));
        new_space[len] = root->character;
        new_space[len + 1] = '\0';
        *buffer = new_space;
        return;
    }

    for( int i = 0; i < root->num_child; i++){
        concatenate(root->children[i], buffer);

    }
}

// Function to free all the nodes given a root.
void free_tree(Node *root){

    if (root->num_child > 0){
        for( int i = 0; i < root->num_child; i++){
            free_tree(root->children[i]);
        }
        free(root->children);
    }
    free(root);
}

// Function to perform merge all valid from paper
// section III-A, Algorithm 1 (high level) line 2
void merge_all_valid(Node *root){

    // Duplicate the tree given the root so we do not affect the origial tree.
    Node *dul_root = calloc(1, sizeof(Node));
    dul_root->capacity = root->capacity;
    dul_root->character = root->character;
    dul_root->t = root->t;
    dul_root->pos = root->pos;
    dul_root->num_child = root->num_child;
    dul_root->children = calloc(dul_root->num_child, sizeof(Node*));

    for( int i = 0; i < dul_root->num_child; i++){
        Node *cur_og = root->children[i];

        Node * node = malloc(sizeof(Node));
        node->parent = dul_root;
        node->capacity = 0;
        node->character = cur_og->character;
        node-> t= -1;
        node->num_child = 0;
        node->pos = i;
        node->children = NULL;
        dul_root->children[i] = node;
    }

    free_tree(dul_root)
}
