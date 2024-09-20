#include "ARVADA.h"

int main (int argc, char **argv){

    // Argument check 
    if ( argc < 2){
        printf("Not enough command line arguments passed in");
    }
    
    // establishing the root node
    Node root = {
        .character = NULL,
        .order_num = 0,
        .num_child = 0,
        .parent = NULL,
    };

    // Getting the name of the file
    char *file_name = *(argv + 1);
    printf("filename = %s", file_name);
    
    // Opening the file in read mode
    FILE *file_ptr;
    file_ptr = fopen(file_name, "r");

    // SomeWhere to store the content
    char line[12];
    
    /* redading the whole code line
    char *line_ptr = fgets(line, sizeof(line) , file_ptr);
    printf("Here is the contents of the file: %s", line);
    */
    
    
    // closing the file
    fclose(file_ptr);
    return 0;
}