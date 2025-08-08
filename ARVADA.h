#ifndef ARVADA_H
#define ARVADA_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

/*
    A node sturct to represent each child, each child will store:
    - capicity
    - a character
    - parent node
    - boolean
    - number of children
    - Array of children
*/
typedef struct node{
    int capicity;
    char character;
    struct node *parent;
    int terminal;
    int num_child;
    struct node **children;
} Node;

/*
    A struct to keep track of all nodes ever produced in code
    - capicity: number of nodes it can currently store
    - count: number of total node
    - nodes: a array of node pointers
*/
typedef struct nodes{
    int capacity;
    int count;
    struct node **nodes;
} Nodes;

void print_all_trees(Nodes *nodes);
void check_node_capicity(Node *node);
void check_nodes_capicity(Nodes *nodes);

#endif
