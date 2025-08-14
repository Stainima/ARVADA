#include "ARVADA.h"

// All steps referenced according to Original paper






//////////////////////////////////////////////////////////////////////////////

int main (int argc, char **argv){

    // Argument check
    if ( argc < 2){
        printf("Not enough command line arguments passed in. \n");
        return EXIT_FAILURE;
    }

     // Step 1: reading in all example sentences
     // and buliding naive parse trees for each sentence
     // Section III-A of original paper

    // Keeping track of all root trees
     // (so each sentence in the sample file)
     Nodes *root_trees= malloc(sizeof(Nodes)); // freed
     root_trees->capacity = 4; // randomly assigned
     root_trees->count = 0;
     root_trees->nodes = malloc(root_trees->capacity * sizeof(Node*)); // freed

     // Getting the file name and reading it in
     char *file_name = *(argv + 1);
     printf("filename = %s\n", file_name);

     FILE *file_ptr;
     file_ptr = fopen(file_name, "r");

     // Error handling for file opening
     if (!file_ptr) {
        fprintf(stderr, "Error opening file: %s\n", file_name);
        return EXIT_FAILURE;
     }

     // reading in line by line
     char *current_line = NULL; // freed
     size_t line_buffer_len = 0;
     ssize_t read_line_len = 0;
     while((read_line_len = getline(&current_line, &line_buffer_len, file_ptr)) != -1){

         current_line[read_line_len - 1] = '\0';
         //printf("Current Line = %s\n", current_line);

         // builidng the navie parse tree for each sentence
         Node *current_tree = malloc(sizeof(Node));
         current_tree->capacity = 10; // randomly assigned
         current_tree->parent = NULL;
         current_tree->t = 0;
         current_tree->num_child = 0;
         current_tree->children = calloc(current_tree->capacity, sizeof(Node*));

         for(int i = 0; i < (read_line_len - 1); i ++ ){
             //printf("%c\n", current_line[i]);
             Node * node = malloc(sizeof(Node));
             node->parent = current_tree;
             node->capacity = 0;
             node->character = current_line[i];
             node-> t= -1;
             node->num_child = 0;
             node->children = NULL;
             current_tree->children[current_tree->num_child] = node;
             current_tree->num_child ++;
             check_node_capacity(current_tree);

         }

         // Check current capacity of the root nodes
         root_trees->nodes[root_trees->count] = current_tree;
         root_trees->count = root_trees->count + 1;
         check_nodes_capacity(root_trees);

     }

     print_all_trees(root_trees);
     free(current_line);
     if(!feof(file_ptr)){
         fprintf(stderr, "Error: Stopped reading before end of the file was reached.");
     }

     fclose(file_ptr);

     // freeing all root nodes
     for( int i = 0; i < root_trees-> count; i ++){

         Node *cur_node = root_trees->nodes[i];
         for( int j = 0; j < cur_node->num_child ; j ++){
             free(cur_node->children[j]);
             }
         free(cur_node);
     }
     free(root_trees->nodes);
     free(root_trees);

    return EXIT_SUCCESS; // Indicate successful execution
}
