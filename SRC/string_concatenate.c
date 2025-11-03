#include "ARVADA.h"
// Functions to do with concatination
// Refer to seciton III-A of the paper

// Function to concatenate the nodes into a string given a root node
void concatenate(Node *root, char** buffer){

    if(root->t_label == -1){
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

void concatenate_with_levels(Node *root, char** buffer, int lvl){

    if(root->t_label == -1){
        size_t len = strlen(*buffer);
        char *new_space = realloc(*buffer, (len + 3) * sizeof(char));
        new_space [len] = lvl + '0';
        new_space[len + 1] = root->character;
        new_space[len + 2] = '\0';
        *buffer = new_space;
        printf("Char: %c.\n", root->character);
        return;
    }

    printf("Labels: %d.\n", root->t_label);
    for( int i = 0; i < root->num_child; i++){
        concatenate_with_levels(root->children[i], buffer, lvl + 1);

    }
}

// Fucntion to concat and print
void concact_and_print(Node *tree){

    char *buffer = calloc(1, sizeof(char));
    concatenate(tree, &buffer);
    printf("Printing buffer: %s.\n",buffer);
    free(buffer);
}

// Fucntion to concat and print
void concact_and_print_with_lvl(Node *tree){

    char *buffer = calloc(1, sizeof(char));
    concatenate_with_levels(tree, &buffer, 0);
    printf("Printing buffer: %s.\n",buffer);
    free(buffer);
}
