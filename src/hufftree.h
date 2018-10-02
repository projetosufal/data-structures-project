#ifndef _HUFFTREE_H_
#define _HUFFTREE_H_
#include <stdbool.h>

// void inserir_ordenado(huff_node **head, huff_node *new_root) {
// 	huff_node *aux = NULL;

	// aux = *head;
	// while (aux->next != NULL && aux->next->freq < new_root->freq) {
	// 	aux = aux->next;
	// }

// 	huff_node *new_node = NULL;
// 	new_node = (huff_node *) malloc(sizeof(huff_node));
// 	new_node->item = item;
// 	new_node->next = aux->next;
// 	aux->next = new_node;
// }


// Function that returns the i'th bit from a byte.
bool get_bit(unsigned char c, int i) {
	return (1 << 7-i) & c;
}

unsigned char set_bit(unsigned char c, int i) {
	return (1 << 7-i) | c;
}

void build_huffman_tree(huff_node **huffman_tree, huff_node *frequency_list) {

	if (frequency_list == NULL || frequency_list->next == NULL) {
		*huffman_tree = frequency_list;
		return;
	}

	// printf("head da lista %p valor: %c e frequencia: %d\n", frequency_list, *((char *)frequency_list->value), frequency_list->freq);
	// printf("head da lista %p tem frequencia: %d\n", frequency_list, frequency_list->freq);

	huff_node *smallest_node = frequency_list;
	huff_node *smallest_node2 = frequency_list->next;
	// printf("menores %p valor %c e %p valor %c\n", smallest_node, (char)smallest_node->value, smallest_node2, (char)smallest_node2->value);

	// remove the two nodes from the list, jumping the head to the 3rd item
	frequency_list = smallest_node2->next;

	// create a new node, sum the two nodes frequencies, point left to the smallest, right to the largest and put it into the list
	huff_node *new_node = create_huff_node("*", smallest_node->freq + smallest_node2->freq, frequency_list, smallest_node, smallest_node2);

	// frequency_list = new_node; // aqui insere desordenado, mas precisa inserir ordendo

	// printf("node novo tem freq %d, left freq %d e right freq %d\n", new_node->freq, new_node->left->freq, new_node->right->freq);

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
	// } else {
	// 	while (aux_node->next != NULL && aux_node->next->freq < new_node->freq) {
	// 		aux_node = aux_node->next;
	// 	}
	// }

	// set the new node next to its position
	new_node->next = aux_node->next;
	aux_node->next = new_node;

	// print out the frequency list
	// huff_node *aux_list = frequency_list;
	// while(aux_list != NULL) {
	// 	// printf("BYTE: %hhx / FREQ: %d\n", *((char *)aux_list->value), aux_list->freq);
	// 	printf("BYTE: %c / FREQ: %d\n", *((char *)aux_list->value), aux_list->freq);
	// 	aux_list = aux_list->next;
	// }

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
		// printf(" ( %c ", (char)huffman_tree->value);
		// printf(" ( %hhx ", (char)huffman_tree->value);
	}

	print_tree(huffman_tree->left);
	print_tree(huffman_tree->right);

	printf(") ");
}

#endif