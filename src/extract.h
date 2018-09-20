#ifndef _EXTRACT_H_
#define _EXTRACT_H_
#include <math.h>
#include "hufflist.h"
#include "hufftree.h"
#include <stdbool.h>

// Function that returns the i'th bit from a byte.
bool get_bit(unsigned char c, int i) {
	unsigned char aux = 1 << i;
	return aux & c;
}

// Recursive function to recreate a tree based on it's pre-order array
huff_node *recreate_tree(char *arr, int *visited, int i) {
	// We need to mark the current position of the array as visited.
	visited[i] = 1;
	/* 
	If the current position in the array is not '*', the current position can only be:
	1. A leaf of the tree;
	2. A backslash escaping a leaf of the tree; 
	*/
	if(arr[i] != '*') {
		if(arr[i] != '\\') {
			unsigned char *c = malloc(sizeof(char));
			*c = arr[i];
			return create_huff_node((void *)c, 0, NULL, NULL, NULL);
		}
		else {
			unsigned char *c = malloc(sizeof(char));
			*c = arr[i+1];
			/*
			If the current character in the array is a backslash, we mark the next character as visited too, 
			because we create a leaf using the next character.
			*/
			visited[i+1] = 1;
			return create_huff_node((void *)c, 0, NULL, NULL, NULL);
		}
	}
	else {
		// When the current character in the array is a non-escaped '*', we know that this node will have both children.
		unsigned char *c = malloc(sizeof(char));
		*c = '*';
		huff_node* root = create_huff_node((void *)c, 0, NULL, NULL, NULL);
		// Since the tree is in preorder, we know that the left child is the character immediately after the current one.
		root->left = recreate_tree(arr, visited, i+1);

		// The right child, on the other hand, will be the next UNVISITED node after the recursion for the left subtree has ended.
		int next_unvisited = i+1;
		while(visited[next_unvisited] != 0) {
			next_unvisited+=1;
		}
		root->right = recreate_tree(arr, visited, next_unvisited);
		return root;
	}
}

void extract(FILE* file) {
	// The two first bytes of the file provides us the information needed to build the tree that will be used
	unsigned char header[2];
	int thrash = 0, tree_size = 0;
	fscanf(file, "%c%c", &header[0], &header[1]);

	// Three fors to organize the reading of the header values.
	for(int i = 5; i < 8; ++i) {
	  thrash += get_bit(header[0], i)*pow(2, i);
	}
	for(int i = 0; i < 8; ++i) {
		tree_size += get_bit(header[1], i)*pow(2, i);
	}
	for(int i = 0; i < 5; ++i) {
		tree_size += get_bit(header[0], i)*pow(2, 8+i);
	}
	
	// Read the tree bytes from file and store those in an array
	unsigned char arr[tree_size];
	int *visited = calloc(tree_size, sizeof(int));
	for(int i = 0; i < tree_size; i++) {
		fscanf(file, "%c", &arr[i]);
	}

	// Recreate the tree based on the preorder array that was read.
	huff_node* tree = recreate_tree(arr, visited, 0);
	
	DEBUG {
		printf("THRASH: %d TREE_SIZE: %d\n", thrash, tree_size);
		print_tree(tree, 0);
	}
}
#endif
