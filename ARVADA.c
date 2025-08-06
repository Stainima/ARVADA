#include "ARVADA.h"

// Remember to docu according to paper
// if the node_master starts overflowing
void check_master(All_nodes* node_master){
     if (node_master->count == node_master->capacity) {
        node_master->capacity += (node_master->capacity / 2);
        node_master->nodes = realloc(node_master->nodes, node_master->capacity * sizeof(Node *));
        if (!node_master->nodes) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
     }
}


int main (int argc, char **argv){

    // Argument check
    if ( argc < 2){
        printf("Not enough command line arguments passed in");
        return EXIT_FAILURE;
    }

    // Mallocing space for all node struct
    All_nodes *node_master = malloc(sizeof(All_nodes));
    node_master->capacity = 80; // Setting initial capicity as 80
    node_master->count = 0; // Initiall no nodes, so coutn = 0
    node_master->nodes = calloc(node_master->count,sizeof(Node*)); // mallocing space for 80 nodes

    // establishing the root node
    // Mallocing space for the node
    Node *root = malloc(sizeof(Node));
    node_master->nodes[0] = root; // keeping track of root node
    node_master->count++; // Incrementing count

    /*

    // Initialising the root nodes.
    root->order_num = 1;
    root->parent = NULL;
    root->num_child = 0;
    root->children = NULL;

    // Getting the name of the file
    char *file_name = *(argv + 1);
    printf("filename = %s\n", file_name);

    // Opening the file in read mode
    FILE *file_ptr;
    file_ptr = fopen(file_name, "r");

    // Check if the file opened successfully
    if (!file_ptr) {
        fprintf(stderr, "Error opening file: %s\n", file_name);
        free(node_master->nodes);
        free(node_master);
        return EXIT_FAILURE;
    }

    // Space for all the children node pointer.
    // Caping at 80 to represent good coding practices of 80 characters per line
    int cap = 80;
    root->children = calloc(cap, sizeof(Node*)); // note: may need to be changed later. (memcpy)
    if (!root->children) {
        fprintf(stderr, "Memory allocation failed for current node\n");
        fclose(file_ptr);
        return EXIT_FAILURE;
    }
    // Reading in the file characters 1 by 1 and inttializing the root tree.
    // Reference: section 3A main Algorithm -> building the naive flat tree.
    char c ;
    printf("Performing part 1: building the naive tree.\n");
    while((c = fgetc(file_ptr)) != EOF){

        // setting up the new node
        Node *curr_node = calloc(1,sizeof(Node));
        curr_node->character = c;
        curr_node->parent = root;

        curr_node->order_num = root->num_child + 1;

        // adding it to node_master
        node_master->nodes[node_master->count] = curr_node;
        node_master->count++;
        check_master(node_master);

        if (root->num_child == cap) {
            cap += cap/ 2;
            root->children = realloc(root->children, cap * sizeof(Node*));
            if (!root->children) {
                fprintf(stderr, "Memory allocation failed for root's children\n");
                fclose(file_ptr);
                return EXIT_FAILURE;
            }
        }


        // adding to the children of the root node
        root->children[root->num_child] = curr_node;
        root->num_child++;
    }
    // closing the file
    fclose(file_ptr);

    // Testing the root
    printf("Children of the root node:\n");
    for(int i = 0; i < root->num_child; i++){
        printf("%c\n", (root->children[i]->character));
    }

     // Freeing all the allocated memory
    for (int i = 0; i < node_master->count; i++) {
        free(node_master->nodes[i]->children); // Free children of each node
        free(node_master->nodes[i]); // Free each node
    }
    free(node_master->nodes); // Free the array of nodes
    free(node_master); // Free the node master structure

    return EXIT_SUCCESS; // Indicate successful execution
}
