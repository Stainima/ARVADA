#include "ARVADA.h"

//---------------------------------------



// Function to perform merge all valid from paper
// section III-A, Algorithm 1 (high level) line 2
// NOTE CURRENTLY PRE-TOKENIZATION IS OFF
void merge_all_valid(Node *root){

    Node *dup_root = duplicate_root(root);
    // Go through the list 1 once
    for( int i = 0; i < dup_root->num_child; i++){

        if (dup_root->children[i]->character == ' '){
            continue;
        }
        // Go through the list number 2 for perms
        for ( int j = i + 1; j < dup_root->num_child; j++){

            Node *tmp = dup_root->children[i];

            // Skip the spaces for now
            if (dup_root->children[j]->character == ' '){
                continue;
            }
            printf("Combs: i = %d, j = %d.\n", i, j);

            merge(tmp, dup_root->children[j], dup_root);
        }

    }

    free_tree(dup_root);

}


// Merge function
int merge(Node *node_1, Node *node_2, Node *dup_tree){

    // If any of the replacement is invalid, it will set a var to false
    int *res = calloc(1, sizeof(int));
    *res = 1;
    replace(node_1, node_2, dup_tree, 0, res);

    if(!(*res)){
        replace(node_2, node_1, dup_tree, 0, res);
    }

    int cur_res = *res;
    free(res);
    return cur_res;
}

// Fucntion to perform sampling string for string replacements
// refer to section III-D, from the original
void replace(Node *replacer, Node *replacee, Node *dup_tree, int pos, int *res){

    //if at any point res become 0, stop execution immediately
    if (!(*res)){
        return;
    }

    // Check if you are replacing single char so you can compare char
    // as terminal do not have a tid ( implementation diff )
    int terminal_replacee = 0;

    if (replacee->t == -1){
        terminal_replacee = 1;
    }

    // Flags to redue calls to the oracle
    int forward = 1;

    // Looping through all the nodes in t0
    for ( int i = pos; i < dup_tree->num_child;  i++){

        Node *cur = dup_tree->children[i];

        // if it is a terminal replaceea and no the correct 1
        // right now. Continue
        if (terminal_replacee){
            if (cur->character != replacee->character){
                continue;
            }
        }

        // perform the swap.
        dup_tree->children[i] = replacer;
        replace(replacer, replacee, dup_tree, i + 1, res);
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
        contact_and_print(dup_tree);
        replace(replacer, replacee, dup_tree, i + 1, res);

    }

    *res = 1;
}

// The Oracle call sand return
int parse_string(const char *input) {
    if (input == NULL) {
        return 0;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 0;
    } else if (pid == 0) {
        // Child: run ./parser with input as argument
        execlp("./parser", "parser", input, (char *)NULL);
        //perror("execlp");
        exit(127);  // exec failed
    } else {
        // Parent: wait for child
        int status;
        if (waitpid(pid, &status, 0) < 0) {
            perror("waitpid");
            return 0;
        }
        if (WIFEXITED(status)) {
            int exitcode = WEXITSTATUS(status);
            if (exitcode == 0) {
                return 1;   // parse success
            } else {
                return 0;   // parse failed (10 or other error)
            }
        } else {
            return 0; // abnormal termination
        }
    }
}
