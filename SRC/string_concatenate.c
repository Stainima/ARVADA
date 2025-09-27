#include "ARVADA.h"
// Functions to do with concatination
// Refer to seciton III-A of the paper

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
    printf("Printing buffer: %s.\n",buffer);
    free(buffer);
}
