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

     // Keeping track of all root nodes
     // (so each sentence in the sample file)
     Nodes *root_nodes = malloc(sizeof(Nodes)); // freed
     root_nodes->capacity = 80; // randomly assigned
     root_nodes->count = 0;
     root_nodes->nodes = calloc(root_nodes->count, sizeof(Node*)); // freed

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
         printf("Current Line = %s\n", current_line);

         // builidng the navie parse tree for each sentence
         Node *current_root = malloc(sizeof(Node));
         current_root->capicity = 80; // randomly assigned
         current_root->parent = NULL;
         current_root->terminal = 0;
         current_root->num_child = 0;
         current_root->children = calloc(current_root->capicity, sizeof(Node*));

         for(int i = 0; i < read_line_len; i ++ ){
             printf("%c\n", current_line[i]);
             Node * node = malloc(sizeof(Node));
             node->parent = current_root;
             node->capicity = 0;
             node->character = current_line[i];
             node->terminal = 1;
             node->num_child = 0;
             node->children = NULL;
             current_root->children[current_root->num_child] = node;
             current_root->num_child ++;

         }

         root_nodes->nodes[root_nodes->count] = current_root;
         root_nodes->count ++;

     }

     free(current_line);
     if(!feof(file_ptr)){
         fprintf(stderr, "Error: Stopped reading before end of the file was reached.");
     }

     fclose(file_ptr);

     // freeing all root nodes
     for( int i = 0; i < root_nodes-> count; i ++){

         Node *cur_node = root_nodes->nodes[i];
         for( int j = 0; j < cur_node->num_child ; j ++){
             free(cur_node->children[j]);
         }
         free(cur_node);
     }
     free(root_nodes->nodes);
     free(root_nodes);

    return EXIT_SUCCESS; // Indicate successful execution
}
