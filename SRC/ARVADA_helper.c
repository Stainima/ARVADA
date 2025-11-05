#include "ARVADA.h"

//---------------------------------------

// Pre-tokenisation
void check_and_tokenise(int cur_class, int *sequence_length, int *sequence_begun, Node **tmp, Node *cur_node){

    // A sequence of the same class as currnet node is in progress.
    if (*sequence_begun == cur_class && *sequence_length > 0){

        // Because the parent of the first child is never assigned,
        // this is done to deal with cases where a sequence of just
        // length 1 is found.
        if (*sequence_length == 1 && (*tmp)->num_child > 0) {
            (*tmp)->children[0]->parent = *tmp;
        }
        // Assigning the current node to tmp,
        // increase sequence number.
        cur_node->parent = *tmp;
        (*tmp)->children[(*tmp)->num_child]= cur_node;
        (*tmp)->num_child ++;
        check_node_capacity(*tmp);
        (*sequence_length)++;

    // no sequence or a different sequence has begun
    }else{

        // different sequence of length 1 is in progress.
        if(*sequence_length == 1){
            // previous sequence was just one node — discard wrapper
            Node *p = cur_node->parent;
            if (p) {
                p->children[p->num_child] = (*tmp)->children[0];
                (p->num_child)++;
                check_node_capacity(p);
                }

            (*tmp)->num_child = 0;
            (*tmp)->children[0] = NULL;
            free((*tmp)->children);
            free_tree(*tmp);
            *tmp= build_basic_node_with_list();
            (*tmp)->children[(*tmp)->num_child] = cur_node;
            (*tmp)->num_child ++;
            check_node_capacity(*tmp);

        // different sequence of length greater than 1 has begun.
        }else if(*sequence_length > 1){
            // finalise previous multi-node sequence
            (*tmp)->parent = (*tmp)->children[0]->parent;
            *tid = *tid + 1;
            (*tmp)->t_label = *tid;


            Node *p = cur_node->parent;
            if (p) {
                p->children[p->num_child] = *tmp;
                (p->num_child)++;
                check_node_capacity(p);
            }

            //concact_and_print(*tmp);
            *tmp = build_basic_node_with_list();
            (*tmp)->children[(*tmp)->num_child] = cur_node;
            (*tmp)->num_child ++;
            check_node_capacity(*tmp);
            *sequence_length = 1;

        // no sequence in progress, assign this node as first node,
        // and begin the sequence.
        }else if(*sequence_begun == 0) {
            (*tmp)->children[(*tmp)->num_child] = cur_node;
            (*tmp)->num_child ++;
            check_node_capacity(*tmp);
            *sequence_length = 1;
        }
    }
    *sequence_begun = cur_class;
    //printf("Sequence Begun: %d.\n", *sequence_begun);
    //printf("sequence_len: %d \n.", *sequence_length);
}

void pre_tokenise(Node* root){

    // Flags to count contigious seuqences.
    int sequence_begun = 0;
    int sequence_length = 0;
    // class 1 = Whitespaces
    // class 2 = Uppercase
    // class 3 = Lowercase
    // class 4 = Digits

    // Set basic node with a list up.
    // parent and children of the node assigned in progress.
    Node *tmp = build_basic_node_with_list();
    int cur_children_num = root->num_child;
    Node ** cur_children = root->children;
    root->capacity = 1;
    root->num_child = 0;
    root->children= calloc(root->capacity, sizeof(Node*));


    for( int i = 0; i < cur_children_num; i++){

        Node * cur_node= cur_children[i];
        //printf("node char: %c.\n", cur_node->character);
        // Current character is a whiteSpace
        if(cur_node->character == ' '){
            check_and_tokenise(1, &sequence_length, &sequence_begun, &tmp, cur_node);

        // Current character is an uppercase character
        }else if(isupper(cur_node->character)){
            check_and_tokenise(2, &sequence_length, &sequence_begun, &tmp, cur_node);

        // Current character is a lower character.
        }else if(islower(cur_node->character)){
            check_and_tokenise(3, &sequence_length, &sequence_begun, &tmp, cur_node);

        // Current character is a digit.
        }else if(isdigit(cur_node->character)){
            check_and_tokenise(4, &sequence_length, &sequence_begun, &tmp, cur_node);

        // Current character is punctuation
        }else{

            if(sequence_length == 1){
                root->children[root->num_child] = tmp->children[0];
                root->num_child ++;
                check_node_capacity(root);
                tmp->num_child = 0;
                tmp->children[0] = NULL;
                free((tmp)->children);
                free_tree(tmp);
            }else if(sequence_length >1 ){
                tmp->parent = tmp->children[0]->parent;
                (*tid) ++;
                tmp->t_label = *tid;

                Node *p = cur_node->parent;
                if (p) {
                    p->children[p->num_child] = tmp;
                    (p->num_child)++;
                    check_node_capacity(p);
                }

            }

            tmp = build_basic_node_with_list();
            root->children[root->num_child] = cur_node;
            root->num_child++;
            check_node_capacity(root);
            sequence_begun = 0;
            sequence_length = 0;
        }
    }
    // deal with leftover seq
    if(sequence_length == 1){
        root->children[root->num_child] = tmp->children[0];
        root->num_child ++;
        check_node_capacity(root);
        tmp->num_child = 0;
        tmp->children[0] = NULL;
        free((tmp)->children);
        free_tree(tmp);
    }else if(sequence_length == 0){
        free_tree(tmp);
    } else  {
        root->children[root->num_child] = tmp;
        root->num_child ++;
        tmp->parent = root;
        (*tid) ++;
        tmp->t_label = *tid;
        check_node_capacity(root);
    }
    free(cur_children);
}


//---------------------------------------


// Function to perform merge all valid from paper
// section III-A, Algorithm 1 (high level) line 2
// NOTE CURRENTLY PRE-TOKENIZATION IS OFF
// No special handling for cases where t_a and t_b are the same
// with non-pre-tokenisation.
void merge_all_valid(Node *root, Nodes *all_trees){

    // Go through the list 1 once
    for( int i = 0; i < root->num_child; i++){

        // Skip White spaces
        if(root->children[i]->character == ' '){
            continue;
        }

        // Go through the list number 2 for perms
        for ( int j = i + 1; j < root->num_child; j++){

            // Skip White spaces
            if(root->children[j]->character == ' '){
                continue;
            }

            // concatenate ta and tb to get strings
            char *buffer_ta = calloc(1, sizeof(char));
            concatenate(root->children[i], &buffer_ta);
            char *buffer_tb = calloc(1, sizeof(char));
            concatenate(root->children[j], &buffer_tb);

            // if both t_a and t_b concatneate to the same string
            // Merge (change labels ) if they are non-terminals or non-white space.
            if(strcmp(buffer_ta,buffer_tb) == 0){
                merge_same_node(root->children[i], root->children[j], i, j, root);
                //printf("Comp: %s, %d\n", buffer_ta, root->children[i]->t_label);
                free(buffer_ta);
                free(buffer_tb);
                continue;
            }

            free(buffer_ta);
            free(buffer_tb);

            // t_a and t_b concatenate to different strings.
            // Now perform a more extensive checl
            int *res = calloc(1, sizeof(int));
            *res = 1;
            basic_replacement_check(root->children[i], root->children[j], all_trees, res);
            basic_replacement_check(root->children[j], root->children[i], all_trees, res);
            printf("Here: %d.\n", *res);

            free(res);

        }

    }

}

// Special function to handle merging attempting of the same string.
void merge_same_node(Node *ta, Node *tb, int i , int j, Node *root){

    // Check the labels have already been merge
    // cases where theere are 3 of the same kind
    if((ta->t_label == tb->t_label) && (ta->t_label > 0)){
        return;
    }

    // if we are trying to merge 2 leaf nodes,
    // This is assuming pre-tokenisation.
    if(ta->t_label == -1 || tb->t_label == -1){

        // Create and link the nodes.
        // Only create the Link node if a link
        // intermediate node does not exist yet.
        // This accounts for when there are multiple of the same
        // leaf node.
        //printf("parent: %d, %d, %d\n", ta->parent->t_label, ta->parent->children[i]->t_label, i);
        if(root->children[i]->t_label == -1){

            Node *ta_label = build_basic_node_with_list();
            (*tid)++;
            ta_label ->t_label = *tid;
            ta_label->parent = ta->parent;
            ta_label->children[0] = ta;
            (ta_label->num_child) ++;
            root->children[i] = ta_label;
            ta->parent = ta_label;
        }
        Node *tb_label = build_basic_node_with_list();
        tb_label ->t_label = *tid;
        tb_label->parent = tb->parent;
        tb_label->children[0] = tb;
        (tb_label->num_child) ++;
        root->children[j] = tb_label;
        tb->parent = tb_label;
        return;
    }

    // if 2 non-terminals are given
    // converge by changing the both t-label to
    // min (ta->t_label, tb->t__label)
    if(ta->t_label < tb->t_label){
        tb->t_label = ta->t_label;
    } else {
        ta->t_label = tb->t_label;
    }
    return;
}


//Merge funciton
// Reference to seciton III-C of the original paper
int merge(Node *node_1, Node *node_2, Node *dup_tree){

    int attempted_merge = validate_merge(node_1, node_2, dup_tree);
    return attempted_merge;
}

// Function to valid merge
int validate_merge(Node *node_1, Node *node_2, Node *dup_tree){

    // If any of the replacement is invalid, it will set a var to false
    int *res = calloc(1, sizeof(int));
    *res = 1;
    advanced_replacement_check(node_1, node_2, dup_tree, 0, res);

    if(!(*res)){
        advanced_replacement_check(node_2, node_1, dup_tree, 0, res);
    }

    int cur_res = *res;
    free(res);
    return cur_res;
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
