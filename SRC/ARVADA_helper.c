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

// Fucntion to concat and print
void contact_and_print(Node *tree){

    char *buffer = calloc(1, sizeof(char));
    concatenate(tree, &buffer);
    int valid = is_while_loop_valid(buffer);
    printf("Printing buffer: %s \n",buffer);
    free(buffer);
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


Node *duplicate_root(Node *root){

    // Duplicate the tree given the root so we do not affect the origial tree.
    Node *dup_root = calloc(1, sizeof(Node));
    dup_root->capacity = root->capacity;
    dup_root->character = root->character;
    dup_root->t = root->t;
    dup_root->pos = root->pos;
    dup_root->num_child = root->num_child;
    dup_root->children = calloc(dup_root->num_child, sizeof(Node*));

    for( int i = 0; i < dup_root->num_child; i++){
        Node *cur_og = root->children[i];

        Node * node = malloc(sizeof(Node));
        node->parent = dup_root;
        node->capacity = 0;
        node->character = cur_og->character;
        node-> t= -1;
        node->num_child = 0;
        node->pos = i;
        node->children = NULL;
        dup_root->children[i] = node;
    }

    return dup_root;

}

// Function to perform merge all valid from paper
// section III-A, Algorithm 1 (high level) line 2
void merge_all_valid(Node *root){

    Node *dup_root = duplicate_root(root);
    // Go through the list 1 once
    for( int i = 0; i < dup_root->num_child; i++){

        if (dup_root->children[i]->character == ' '){
            continue;
        }
        // Go through the list number 2 for perms
        for ( int j = i + 1; j < dup_root->num_child; j++){

            Node *tmp = dup_root->children[i];

            // Skip the spaces for now
            if (dup_root->children[j]->character == ' '){
                continue;
            }

            merge(tmp, dup_root->children[j], dup_root);
        }

    }

    free_tree(dup_root);

}


// Merge function
int merge(Node *node_1, Node *node_2, Node *dup_tree){

    int *res = calloc(1, sizeof(int));
    *res = 1;
    replace(node_1, node_2, dup_tree, 0, res);

    free(res);
    return 1;
}

// Fucntion to perform sampling string for string replacements
// refer to section III-D, from the original
void replace(Node *replacer, Node *replacee, Node *dup_tree, int pos, int *res){

    // Check if you are replacing single char so you can compare char
    // as terminal do not have a tid ( implementation diff )
    int terminal_replacee = 0;
    if (replacee->t == -1){
        terminal_replacee = 1;
    }

    // Flags to redue calls to the oracle
    int forward = 1;

    // Looping through all the nodes in t0
    for ( int i = pos; i < dup_tree->num_child;  i++){

        Node *cur = dup_tree->children[i];

        // if it is a terminal replaceea and no the correct 1
        // right now. Continue
        if (terminal_replacee){
            if (cur->character != replacee->character){
                continue;
            }
        }

        // perform the swap.
        dup_tree->children[i] = replacer;
        replace(replacer, replacee, dup_tree, i + 1, res);
        if(forward){
            // call to oracle then
            // if (call to oracle) -> pass : *res = 0;
            contact_and_print(dup_tree);
            forward = 0;
        }
        dup_tree->children[i] = cur;
        replace(replacer, replacee, dup_tree, i + 1, res);

    }

    *res = 1;
}
