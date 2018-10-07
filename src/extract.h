#ifndef _EXTRACT_H_
#define _EXTRACT_H_
#include <math.h>
#include "hufflist.h"
#include "hufftree.h"
#include "compress.h"
#include <stdbool.h>

int balance(huff_node *root, int height) {
  if(root->left == NULL && root->right == NULL) {
    return 1;
  }
  return balance(root->left, height+1) - balance(root->right, height+1);
}

// Recursive function to recreate a tree based on it's pre-order array
huff_node *recreate_tree(byte *arr, int *visited, int i, int tree_size) {
	// We need to mark the current position of the array as visited.
	visited[i] = 1;
	/*
	If the current position in the array is not '*', the current position can only be:
	1. A leaf node;
	2. A backslash escaping a leaf node; 
	*/
	if(i < tree_size) {
		if(arr[i] != '*') {
			if(arr[i] != '\\') {
				byte *c = malloc(sizeof(char));
				*c = arr[i];
				return create_huff_node((void *)c, 0, NULL, NULL, NULL);
			}
			else {
				byte *c = malloc(sizeof(char));
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
			byte *c = malloc(sizeof(char));
			*c = '*';
			huff_node* root = create_huff_node((void *)c, 0, NULL, NULL, NULL);

			// Since the tree is in preorder, we know that the left child is the character immediately after the current one.
			root->left = recreate_tree(arr, visited, i+1, tree_size);

			// The right child, on the other hand, will be the next UNVISITED node after the recursion for the left subtree has ended.
			int next_unvisited = i+1;
			while(visited[next_unvisited] != 0) {
				next_unvisited+=1;
			}
			root->right = recreate_tree(arr, visited, next_unvisited, tree_size);
			return root;
		}
	}
  return NULL;
}

void create_extracted_file(FILE *extracted_file, FILE *original_file, int thrashless_size, int thrash, huff_node *root) {
	int current_bit = 0;
	byte current_byte;
	huff_node* current_node = root;
	
	DEBUG {
		printf("CURRENT %d THRASH %d\n", current_bit, thrashless_size);
	} 
	while(fscanf(original_file, "%c", &current_byte) != EOF) {
		for(int i = 0; i < 8 && current_bit <= thrashless_size; ++i, ++current_bit) {
			DEBUG {
				printf("%d", get_bit(current_byte, i));
			}
			if(current_node->left == NULL && current_node->right == NULL) {
				fprintf(extracted_file, "%c", *((byte *)current_node->value));
				current_node = root;
			}
			if(get_bit(current_byte, i)) {
				current_node = current_node->right;
			}
			else {
				current_node = current_node->left;
			}
		}
	}
	if(thrash == 0 && balance(root, 0) == 0) {
		fprintf(extracted_file, "%c", *((byte *)current_node->value));
	}
	DEBUG {
    printf("%d\n", balance(root, 0));
		puts("");
	}
}

void extract(char *filename) {
	// Try to load the file that will be extracted
	FILE *file = fopen(filename, "r");
	if(file == NULL) {
		printf("Error! File could not be loaded successfully.\nExiting...\n");
		return;
	}

	// Try to load the extracted file
	int extracted_filesize = strlen(filename) - 5;
	char *extracted_filename = malloc(extracted_filesize * sizeof(char));
	strcpy(extracted_filename, filename);
	extracted_filename[extracted_filesize] = '\0';
	FILE *extracted_file = fopen(extracted_filename, "r");
	
	while(extracted_file != NULL) {
		printf("Extracted file already exists.\nEnter a new file name (including extension):\n");
		char new_filename[255];
		scanf("%s", new_filename);
		new_filename[strlen(new_filename)] = '\0';
		fclose(extracted_file);
		extracted_file = fopen(new_filename, "r");
		strcpy(extracted_filename, new_filename);
	}
	extracted_file = fopen(extracted_filename, "w");
	printf("Extracting file...\n");

	// The two first bytes of the file provides us the information needed to build the tree that will be used
	byte header[2];
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
		tree_size += get_bit(header[0], i)*pow(2, 8+(7-i));
	}

	// Read the tree bytes from file and store those in an array
	byte arr[tree_size];
	int *visited = calloc(tree_size, sizeof(int));
	for(int i = 0; i < tree_size; ++i) {
		fscanf(file, "%c", &arr[i]);
	}

	// Recreate the tree based on the preorder array that was read, and then get the file size.
	huff_node* huffman_tree = recreate_tree(arr, visited, 0, tree_size);
	fseek(file, 0L, SEEK_END);
	long file_size = ftell(file);
	// Rewind the file and escape the header.

	rewind(file);
	byte thrash1, thrash2;
	fscanf(file, "%c%c", &thrash1, &thrash2);
	for(int i = 0; i < tree_size; i++) {
		fscanf(file, "%c", &thrash1);
	}

	DEBUG {
		printf("FILESIZE: %ld THRASH: %d TREE_SIZE: %d\n", file_size, thrash_size, tree_size);
		print_tree(huffman_tree);
		puts("");
	}

	// Recreate the extracted file using the tree
	create_extracted_file(extracted_file, file, ((file_size - 2 - tree_size) * 8 - thrash_size), 0, huffman_tree);

	fclose(file);
	fclose(extracted_file);
	printf("Freeing used memory...\n");
	free_tree(&huffman_tree);
	printf("Done.\n");
}
#endif