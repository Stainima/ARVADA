#ifndef ARVADA_H
#define ARVADA_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

/*
    A node sturct to represent each child, each child will store:
    - capacity
    - a character
    - parent node
    - boolean
    - number of children
    - number of the position of the node in the tree
    - Array of children
*/
typedef struct node{
    int capacity;
    char character;
    struct node *parent;
    int t;
    int num_child;
    int pos;
    struct node **children;
} Node;

/*
    A struct to keep track of all nodes ever produced in code
    - capacity: number of nodes it can currently store
    - count: number of total node
    - nodes: a array of node pointers
*/
typedef struct nodes{
    int capacity;
    int count;
    struct node **nodes;
} Nodes;

void print_all_trees(Nodes *nodes);
void check_node_capacity(Node *node);
void check_nodes_capacity(Nodes *nodes);
void concatenate(Node *root, char** buffer);
void free_tree(Node *root);
int validate_while_program(const char* input);

#endif
