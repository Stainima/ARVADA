
// This is chat Gpt's respone.
void Bubble(Node *parent, int start, int end) {
    // Ensure valid range
    if (start >= end || start < 0 || end > parent->num_child) {
        printf("Invalid bubble range\n");
        return;
    }

    // Create the bubble node
    Node *bubble_node = malloc(sizeof(Node));
    bubble_node->parent = parent;     // The parent is still the original parent
    bubble_node->num_child = end - start;  // The number of bubbled children
    bubble_node->children = malloc((end - start) * sizeof(Node *));  // Space for children

    // Transfer children to bubble_node
    for (int i = start; i < end; i++) {
        Node *child = parent->children[i];
        child->parent = bubble_node;          // Update parent to bubble_node
        bubble_node->children[i - start] = child; // Move child to the bubble node
    }

    // Adjust the original parent's children array, replacing bubbled children
    // First, compact the array by shifting nodes that come after the bubbled segment
    for (int i = end; i < parent->num_child; i++) {
        parent->children[i - (end - start - 1)] = parent->children[i];
    }

    // Insert the bubble_node in place of the original first bubbled child
    parent->children[start] = bubble_node;

    // Adjust the child count in the parent node
    parent->num_child -= (end - start - 1);

    printf("Bubbled nodes from index %d to %d\n", start, end);
}

// Assume you want to bubble the first 3 children of the root node
Bubble(root, 0, 3);
        
/*
Notes:
Generalization: After bubbling, you would need to check if the new bubble can generalize (e.g., if it allows for a valid grammar structure). 
This is typically done using an oracle as described in the paper.

Recursive Bubbling: ARVADA uses this bubbling operation recursively to build more structure into the grammar,
so you could iteratively or recursively apply this process to nested sequences of siblings.

Two-way Merging: After bubbling, ARVADA tries to merge the newly introduced nonterminal with existing ones to introduce recursion.
You may want to implement this merging logic later, depending on your needs.

This provides a way to implement the core bubbling mechanism. Let me know if you need more detail on merging or recursion aspects.

*/