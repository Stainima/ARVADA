#ifndef ARVADA_H
#define ARVADA_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* 
    A node sturct to represent each child, each child will store:
    - a character
    - the order of the character
    - parent node
    - number of children of the current node
    - Array of children
*/
typedef struct node{
    char character;
    int order_num;
    struct node *parent;
    int num_child;
    struct node **children;
} Node ;

/*
    A struct to keep track of all nodes ever produced in code
    - capicity: number of node pointer it can accomodate for now
    - count: number of total node
    - nodes: a array of node pointers
*/
typedef struct all_nodes{
    int capicity;
    int count;
    struct node **nodes;
} All_nodes;

#endif