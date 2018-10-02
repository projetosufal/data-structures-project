#ifndef _HUFFTREE_H_
#define _HUFFTREE_H_
#include <stdbool.h>

// Function that returns the i'th bit from a byte.
bool get_bit(unsigned char c, int i) {
	return (1 << (7-i)) & c;
}

unsigned char set_bit(unsigned char c, int i) {
	return (1 << (7-i)) | c;
}

void build_huffman_tree(huff_node **huffman_tree, huff_node *frequency_list) {

	if (frequency_list == NULL || frequency_list->next == NULL) {
		*huffman_tree = frequency_list;
		return;
	}

	huff_node *smallest_node = frequency_list;
	huff_node *smallest_node2 = frequency_list->next;

	// remove the two nodes from the list, jumping the head to the 3rd item
	frequency_list = smallest_node2->next;

	// create a new node, sum the two nodes frequencies, point left to the smallest, right to the largest and put it into the list
	huff_node *new_node = create_huff_node("*", smallest_node->freq + smallest_node2->freq, frequency_list, smallest_node, smallest_node2);

	// find the place to insert ordered the new node into the list
	huff_node *aux_node = frequency_list;
	while (aux_node != NULL && aux_node->next != NULL && aux_node->next->freq < new_node->freq) {
		aux_node = aux_node->next;
	}

	if (aux_node == NULL) {
		frequency_list = new_node;
		*huffman_tree = frequency_list;
		return;
	} 

	// set the new node next to its position
	new_node->next = aux_node->next;
	aux_node->next = new_node;
	build_huffman_tree(huffman_tree, frequency_list);
}

/*
Function that does a binary search on the given tree and returns the steps taken to find the node (which is the new alias for the byte).
When the recursion goes to the left child node, we add a 0 to the current alias, when it goes to the right child node, we add a 1.

This function changes the "result" pointer instead of returning.
*/
void search_tree(huff_node *root, unsigned char c, huff_node *current_alias, char **result) {
		if(root != NULL) {
			if(root->left == NULL && root->right == NULL) {
				if(*((char *)root->value) == c) {
					to_string(current_alias, result);
				}
				return;
			}
			if(root != NULL && root->left != NULL) {
				char *char_to_add = malloc(sizeof(char));
				*char_to_add = '0';
				current_alias = add_to_tail(current_alias, (void *)char_to_add, 0);
					search_tree(root->left, c, current_alias, result);
					current_alias = remove_from_tail(current_alias);
			}
			if(root != NULL && root->right != NULL) {
				char *char_to_add = malloc(sizeof(char));
				*char_to_add = '1';
				current_alias = add_to_tail(current_alias, (void *)char_to_add, 0);
					search_tree(root->right, c, current_alias, result);
					current_alias = remove_from_tail(current_alias);
			}
		}
		else {
			DEBUG {
				printf("NULL root passed to search_tree.\n");
			}
		}
}

void get_tree_size(huff_node *root, int *size) {
	if(root == NULL) {
		return;
	}
	*size += 1;
	get_tree_size(root->left, size);
	get_tree_size(root->right, size);
}

void write_preorder(FILE *file, huff_node *root) {
	if(root == NULL) {
		return;
	}
	if(root->left == NULL && root->right == NULL &&
	   (*((char *)root->value) == '*' || *((char *)root->value) == '\\')) {
		fprintf(file, "\\%c", *((char *)root->value));
	}
	else {
		fprintf(file, "%c", *((char *)root->value));
	}
	write_preorder(file, root->left);
	write_preorder(file, root->right);
}

void print_tree(huff_node *huffman_tree) {

	if (huffman_tree == NULL) {
		printf(" () ");
		return;
	} else {
		printf(" ( %d %c", (int)huffman_tree->freq, *((char*)huffman_tree->value));
	}

	print_tree(huffman_tree->left);
	print_tree(huffman_tree->right);

	printf(") ");
}
#endif