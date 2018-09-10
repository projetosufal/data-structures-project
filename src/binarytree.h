#ifndef _BINARYTREE_
#define _BINARYTREE_

typedef struct huf_tree {
	void *value;
	int height;
	struct huf_tree *left;
	struct huf_tree *right;
} huf_tree;

// Function that allocates memory and returns the address for a new tree node.
huf_tree *create_tree_node(void *item, int height, huf_tree *left, huf_tree *right) {
	huf_tree *new_node = malloc(sizeof(huf_tree));
	new_node->value = item;
	new_node->height = height;
	new_node->left = left;
	new_node->right = right;
	return new_node;
}

#endif