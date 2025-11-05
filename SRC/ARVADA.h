#ifndef ARVADA_H
#define ARVADA_H

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <ctype.h>

// Making tid global
extern int *tid;

/*
    A node sturct to represent each child, each child will store:
    - capacity
    - a character
    - parent node
    - boolean
    - number of children (note this coould also be used for coverage range of non-terminals)
    - number of the position of the node in the tree
    - Array of children
*/
typedef struct node{
    int capacity;
    char character;
    struct node *parent;
    int t_label;
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
    struct node **rootNodes;
} Nodes;

// ----- From prase tree builder
Node *build_basic_node();
Node *build_basic_node_with_list();
void print_all_trees(Nodes *nodes);
void check_node_capacity(Node *node);
void check_nodes_capacity(Nodes *nodes);
void free_tree(Node *root);
Node *duplicate_tree(Node *root);


// ------ From ARVADA helper
void pre_tokenise(Node *root);
void merge_all_valid(Node *root, Nodes *all_trees);
int merge(Node *node_1, Node *node_2, Node *dup_tree);
int validate_merge(Node *node_1, Node *node_2, Node *dup_tree);
void advanced_replacement_check(Node *replacer, Node *replacee, Node *dup_tree, int pos, int *res);
void basic_replacement_check(Node *ta, Node *tb, Nodes *all_trees, int *res);
void merge_same_node(Node *ta, Node *tb, int i, int j, Node* root);
int parse_string(const char* input);

// ------- From String Concat
void concatenate(Node *root, char** buffer);
void concact_and_print(Node *tree);
void concact_and_print_with_lvl(Node *tree);
void concact_and_print_with_labels(Node *tree);

#endif
