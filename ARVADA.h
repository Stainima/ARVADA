#ifndef ARVADA_H
#define ARVADA_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* A node sturct to represent each child, each child will store:
    - a character
    - the order of the character
    - parent node
    - */
struct node{
    char character[1];
    int order_num;
    node *parent;
    int num_child;
    node **children;
};

#endif
