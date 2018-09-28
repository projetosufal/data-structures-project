#ifndef _COMPRESS_H_
#define _COMPRESS_H_
#include <stdbool.h>
#include "hufflist.h"
#include "hufftree.h"

// Function to create a table with the frequencies of each byte of the file as the value.
void create_table(FILE *file, int *table) {
	unsigned char *current_byte = malloc(sizeof(char));
	while(fscanf(file, "%c", current_byte) != EOF) {
		table[*current_byte] += 1;
	}
	free(current_byte);
}

// Function to change the contents of the "header" array using set_bit() to match the desired header configuration.
void create_header(int thrash_size, int tree_size, char *header) {
	for(int i = 2; i >= 0; i--) {
		if(thrash_size % 2) {
			header[0] = set_bit(header[0], i);
		}
		thrash_size /= 2;
	}
	for(int i = 7; i >= 0; i--) {
		if(tree_size % 2) {
			header[1] = set_bit(header[1], i);
		}
		tree_size /= 2;
	}
	for(int i = 7; i >= 3; i--) {
		if(tree_size % 2) {
			header[0] = set_bit(header[0], i);
		}
		tree_size /= 2;
	}
}

void create_compressed_file(char *filename, huff_node *root) {
	int filename_length = strlen(filename);
	char compacted_filename[filename_length+5];
	strcpy(compacted_filename, filename);
	compacted_filename[filename_length] = '.';
	compacted_filename[filename_length+1] = 'h';
	compacted_filename[filename_length+2] = 'u';
	compacted_filename[filename_length+3] = 'f';
	compacted_filename[filename_length+4] = 'f';
	compacted_filename[filename_length+5] = '\0';
	FILE *file = fopen(compacted_filename, "r"); 
	if(file != NULL) {
		printf("Error! Decompressed file already exists.\nExiting...\n");
		return;
	}
	file = fopen(compacted_filename, "w");
	int thrash_size = get_thrash_size(filename, root);
	int tree_size = 0;
	get_tree_size(root, &tree_size);
	char *header = calloc(2, sizeof(char));
	
	create_header(thrash_size, tree_size, header);
	for(int i = 0; i < 8; i++) {
		printf("%d", get_bit(header[0], i));
	}
	for(int i = 0; i < 8; i++) {
		printf("%d", get_bit(header[1], i));
	}
	puts("");
	fclose(file);
}

void compress(char *filename) {
	// Open the file
	FILE *file = fopen(filename, "r");
	if(file == NULL) {
		printf("Error! File could not be loaded successfully.\nExiting...\n");
		return;
	}

  	// Allocate the memory for our character table, create it and create a list of the frequencies, then sort it
	int *table = calloc(256, sizeof(int));
	
	huff_node *huffman_tree = malloc(sizeof(huff_node));
	
	create_table(file, table);
	
	huff_node *head = create_list(table);
	
	sort_frequency(&head);
	
	build_huffman_tree(&huffman_tree, head);
	
	create_compressed_file(filename, huffman_tree);

	DEBUG {
		print_tree(huffman_tree);
		puts("");
	}

	// create a new file, insert header (tree size, tree preorder)

	// parse the file bits and start compressing using the tree

	DEBUG {
	  while(head != NULL) {
	    printf("BYTE: %c FREQ: %d\n", *((char *)head->value), head->freq);
	    head = head->next;
	  }
	}

	fclose(file);
}
#endif
