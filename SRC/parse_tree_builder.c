#include "ARVADA.h"

// ------------- Funcitons to help build the original naive trees
// Section III-A of original paper

// Fucntion to that buids a basic node
Node *build_basic_node(){

    Node * node = malloc(sizeof(Node));
    node->parent = NULL;
    node->capacity = 0;
    node->character = '\0';
    node-> t_label= -1;
    node->num_child = 0;
    node->pos = -1;
    node->children = NULL;
    return node;
}

// Fucntion to that buids a basic node
Node *build_basic_node_with_list(){

    Node * node = malloc(sizeof(Node));
    node->parent = NULL;
    node->capacity = 1;
    node->character = '\0';
    node-> t_label= -2;
    node->num_child = 0;
    node->pos = -1;
    node->children= calloc(node->capacity, sizeof(Node*));
    return node;
}

// Check and deal with the capicty of root node.
void check_nodes_capacity(Nodes *nodes){

    if(nodes->capacity > nodes->count){
        return;
    }

    // If number of childern has reached capacity, update cap.
    int cur_cap = nodes->capacity;
    nodes->capacity = (cur_cap + (cur_cap/2) + 1);
    void *new_space = realloc(nodes->rootNodes,nodes->capacity * sizeof(Node*));
    if(errno == ENOMEM || new_space == NULL){
        fprintf(stderr, "Error increasing the capicty of nodes.");
        return;
    }
    nodes->rootNodes= new_space;
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
        Node *cur = nodes->rootNodes[i];
        printf("Tree No.%d:",i);
        for(int j = 0; j < cur->num_child; j++){
            printf("%c",cur->children[j]->character);
        }
        printf("\n");
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


// Funciton that will duiplicate the root, including all children given a root
Node *duplicate_tree(Node *root){

    // Duplicate the tree given the root so we do not affect the origial tree.
    Node *dup_root = build_basic_node();
    dup_root->capacity = root->capacity;
    dup_root->parent = root->parent;
    dup_root->character = root->character;
    dup_root->t_label = root->t_label;
    dup_root->pos = root->pos;
    dup_root->num_child = root->num_child;

    if(root->children == NULL && root->t_label == -1){
        return dup_root;
    }

    dup_root->children = calloc(dup_root->num_child, sizeof(Node*));

    for( int i = 0; i < dup_root->num_child; i++){

        Node * node = duplicate_tree(root->children[i]);
        node->parent = dup_root;
        dup_root->children[i] = node;
    }

    return dup_root;

}
