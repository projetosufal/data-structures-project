#ifndef _EXTRACT_H_
#define _EXTRACT_H_
#include <math.h>
#include "hufflist.h"
#include "hufftree.h"
#include "compress.h"
#include <stdbool.h>

// Recursive function to recreate a tree based on it's pre-order array
huff_node *recreate_tree(unsigned char *arr, int *visited, int i) {
	// We need to mark the current position of the array as visited.
	visited[i] = 1;
	/* 
	If the current position in the array is not '*', the current position can only be:
	1. A leaf node;
	2. A backslash escaping a leaf node; 
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

void create_extracted_file(FILE *extracted_file, FILE *original_file, int thrashless_size, huff_node *root) {
	int current_bit = 0, bit_i = 0;
	unsigned char current_byte;
	fscanf(original_file, "%c", &current_byte);
	huff_node* current_node = root;
	
	printf("CURRENT %d THRASHLESS %d\n", current_bit, thrashless_size);
	while(current_bit < thrashless_size) {
		DEBUG {
			printf("%d", get_bit(current_byte, bit_i));
		}
		if(bit_i == 8) {
			fscanf(original_file, "%c", &current_byte);
			bit_i = 0;
		}
		if(current_node->left == NULL && current_node->right == NULL) {
			fprintf(extracted_file, "%c", *(unsigned char *)current_node->value);
			current_node = root;
		}
		if(get_bit(current_byte, bit_i)) {
			current_node = current_node->right;
		}
		else {
			current_node = current_node->left;
		}

		bit_i += 1;
		current_bit += 1;
	}
	DEBUG {
		puts("");
	}
	printf("Done.\n");
}

void extract(char *filename) {
	// Try to load the file that will be extracted
	FILE *file = fopen(filename, "r");
	if(file == NULL) {
		printf("Error! File could not be loaded successfully.\nExiting...\n");
		return;
	}

	// Try to load the extracted file
	int extracted_filesize = strlen(filename) - 4;
	unsigned char *extracted_filename = malloc(extracted_filesize * sizeof(char));
	strcpy(extracted_filename, filename);
	extracted_filename[extracted_filesize-1] = 'x';
	extracted_filename[extracted_filesize] = '\0';
	FILE *extracted = fopen(extracted_filename, "r");
	if(extracted != NULL) {
		printf("Error! extracted file already exists.\nExiting...\n");
		return;
	}
	extracted = fopen(extracted_filename, "w");

	// The two first bytes of the file provides us the information needed to build the tree that will be used
	unsigned char header[2];
	int thrash_size = 0, tree_size = 0;
	fscanf(file, "%c%c", &header[0], &header[1]);

	// Three fors to organize the reading of the header values.
	for(int i = 0; i < 3; ++i) {
	  thrash_size += get_bit(header[0], i)*pow(2, 2-i);
	}
	for(int i = 0; i < 8; ++i) {
		tree_size += get_bit(header[1], i)*pow(2, 7-i);
	}
	for(int i = 3; i < 8; ++i) {
		tree_size += get_bit(header[0], i)*pow(2, 7+(5-i));
	}
	
	// Read the tree bytes from file and store those in an array
	unsigned char arr[tree_size];
	int *visited = calloc(tree_size, sizeof(int));
	for(int i = 0; i < tree_size; i++) {
		fscanf(file, "%c", &arr[i]);
	}

	// Recreate the tree based on the preorder array that was read, and then get the file size.
	huff_node* tree = recreate_tree(arr, visited, 0);
	fseek(file, 0L, SEEK_END);
	long file_size = ftell(file);

	// Rewind the file and escape the header.
	rewind(file);
	unsigned char thrash;
	fscanf(file, "%c%c", &thrash, &thrash);
	for(int i = 0; i < tree_size; i++) {
		fscanf(file, "%c", &thrash);
	}

	// Recreate the extracted file using the tree
	create_extracted_file(extracted, file, ((file_size - 2 - tree_size) * 8) - thrash_size+1, tree);
	
	DEBUG {
		printf("FILESIZE: %ld THRASH: %d TREE_SIZE: %d\n", file_size, thrash_size, tree_size);
		print_tree(tree);
		puts("");
	}
	fclose(file);
}
#endif
