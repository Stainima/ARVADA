#include "ARVADA.h"


// Simple replace function that free later nodes, and replaces it with another node.
// Given 1 nodes, and the index of the second node and root node
void replace(Node *replacer, int j, Node *root){

    free(root->children[j]);
    root->children[j] = replacer;
}

// Froming a basic replacement check,
// replace all instance of t_a with t_b in all trees
// check if the all concatenate to a valid string (oracle)
// do the vise-verse
void basic_replacement_check(Node *ta, Node *tb, Nodes *all_trees, int *res){

    // Concatenate to get the stringstring
    char *buffer_ta = calloc(1, sizeof(char));
    concatenate(ta, &buffer_ta);
    char *buffer_tb = calloc(1, sizeof(char));
    concatenate(tb,  &buffer_tb);

    // loop through all trees.
    for( int i = 0; i < all_trees->count; i++){

        // duplicate tree
        Node *dup_tree = duplicate_tree(all_trees->rootNodes[i]);

        // loop through duplicate tree children node
        for (int j = 0; j < dup_tree->num_child; j++){

            // get current children nodes string.
            char *buffer_tc = calloc(1, sizeof(char));
            concatenate(dup_tree->children[j], &buffer_tc);

            // if check to find correct replacement in the duplciate tree
            if(strcmp(buffer_tb, buffer_tc) ==0){
                replace(ta, j, dup_tree);
            }
            free(buffer_tc);
        }

        // Now all instances are replaces,
        // send to duplicate to oracle to check and return
        char *buffer_dup_tree= calloc(1, sizeof(char));
        concatenate(dup_tree,  &buffer_dup_tree);
        *res = parse_string(buffer_dup_tree);
        if( *res == 0 ){
            return;
        }
        free(buffer_dup_tree);
    }

    free(buffer_ta);
    free(buffer_tb);
}

// Fucntion to perform sampling string for string replacements
// refer to section III-D, from the original
void advanced_replacement_check(Node *replacer, Node *replacee, Node *dup_tree, int pos, int *res){

    //if at any point res become 0, stop execution immediately
    if (!(*res)){
        return;
    }

    // Check if you are replacing single char so you can compare char
    // as terminal do not have a tid ( implementation diff )
    int terminal_replacee = 0;

    if (replacee->t_label == -1){
        terminal_replacee = 1;
    }

    // Flags to redue calls to the oracle
    int forward = 1;

    // Looping through all the nodes in t0
    for ( int i = pos; i < dup_tree->num_child;  i++){

        Node *cur = dup_tree->children[i];

        // if it is a terminal replacee and no the correct 1
        // right now. Continue
        if (terminal_replacee){
            if (cur->character != replacee->character){
                continue;
            }
        }

        // perform the swap.
        dup_tree->children[i] = replacer;
        advanced_replacement_check(replacer, replacee, dup_tree, i + 1, res);
        if(forward){
            // call to oracle then
            // if (call to oracle) -> pass : *res = 0;
            char *buffer = calloc(1, sizeof(char));
            concatenate(dup_tree, &buffer);
            *res = parse_string(buffer);
            printf("Printing buffer: %s and %d.\n",buffer, *res);
            free(buffer);
            forward = 0;

        }
        dup_tree->children[i] = cur;
        // case where 1 of the candidate string is invalid, so return.
        if(!*res){
            return;
        }
        concact_and_print(dup_tree);
        advanced_replacement_check(replacer, replacee, dup_tree, i + 1, res);

    }

    *res = 1;
}
