#ifndef ARVADA_H
#define ARVADA_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

/*
    A node sturct to represent each child, each child will store:
    - a character
    - the order of the character
    - parent node
    - Array of children
*/
typedef struct node{
    char character;
    struct node *parent;
    int num_child;
    struct node **children;
} Node;

/*
    A struct to keep track of all nodes ever produced in code
    - capicity: number of node pointer it can accomodate for now
    - count: number of total node
    - nodes: a array of node pointers
*/
typedef struct all_nodes{
    int capacity;
    int count;
    struct node **nodes;
} All_nodes;

#endif
