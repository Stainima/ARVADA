#include "ARVADA.h"

int main (int argc, char **argv){

    // Argument check 
    if ( argc < 2){
        printf("Not enough command line arguments passed in");
    }
    
    // Getting the name of the file
    char *file_name = *(argv + 1);
    printf("filename = %s", file_name);
    
    // Opening the file in read mode
    FILE *file_ptr;
    file_ptr = fopen(file_name, "r");

    // Somehere to store the content
    char line[12];
    char *line_ptr = fgets(line, sizeof(line) , file_ptr);
    printf("Here is the contents of the file: %s", line);
    
    // closing the file
    fclose(file_ptr);
    return 0;
}