#ifndef _COMPRESS_H_
#define _COMPRESS_H_
#include <stdbool.h>
#include "hufflist.h"
#include "hufftree.h"

// Function to create a table with the frequencies of each byte of the file as the value.
void create_table(FILE *file, int *table) {
	byte current_byte;
	while(fscanf(file, "%c", &current_byte) != EOF) {
		table[current_byte] += 1;
	}
}

// Function to change the contents of the "header" array using set_bit() to match the desired header configuration.
void create_header(int thrash_size, int tree_size, byte *header) {
	DEBUG { 
		printf("\nTREE SIZE: %d TRASH SIZE: %d\n", tree_size, thrash_size);
	}
	for(int i = 2; i >= 0; --i) {
		if(thrash_size % 2) {
			header[0] = set_bit(header[0], i);
		}
		thrash_size /= 2;
	}
	for(int i = 7; i >= 0; --i) {
		if(tree_size % 2) {
			header[1] = set_bit(header[1], i);
		}
		tree_size /= 2;
	}
	for(int i = 7; i >= 3; --i) {
		if(tree_size % 2) {
			header[0] = set_bit(header[0], i);
		}
		tree_size /= 2;
	}

	DEBUG {
		for(int i = 0; i < 8; i++) {
			printf("%d", get_bit(header[0], i));
		}
		printf(" ");

		for(int i = 0; i < 8; i++) {
			printf("%d", get_bit(header[1], i));
		}
		printf("\n");
	}
}

void create_compressed_file(FILE *compressed_file, FILE *original_file, huff_node *root) {
	// Empty header that will be overwritten when the real thrash size is discovered.
	byte *header = calloc(2, sizeof(char));
	create_header(0, 0, header);
	fprintf(compressed_file, "%c%c", header[0], header[1]);

	write_preorder(compressed_file, root);
	byte byte_to_search;
	byte *current_byte = calloc(1, sizeof(char));
	short int bit_i = 0, thrash_size = 0, bytes = 0, current_bit = 0;

	// This while loop runs once for each byte in the original file.
	while(fscanf(original_file, "%c", &byte_to_search) != EOF) {
		char *string_to_add = malloc(sizeof(char));
		search_tree(root, byte_to_search, NULL, &string_to_add);
		
		int i = 0;
		while(i < strlen(string_to_add)) {
			/*
			Since we can only write a byte at a time, the program writes everytime the "bit_i" counter hits 8,
			then the counter is reset to 0.
			*/
			if(bit_i == 8) {
				fprintf(compressed_file, "%c", *current_byte);
				current_byte = calloc(1, sizeof(char));
				bytes += 1;
				bit_i = 0;
			}
			if(string_to_add[i] == '1') {
				*current_byte = set_bit(*current_byte, bit_i);
			}
			current_bit += 1;
			thrash_size += 1;
			bit_i += 1;
			i += 1;
		}
    free(string_to_add);
	}
	fprintf(compressed_file, "%c", *current_byte);

	// The compressed file needs to be rewinded so that we can update the header with the new thrash size.
	rewind(compressed_file);

	// Calculate the thrash size.
	thrash_size = 8 - thrash_size % 8;

	// Update the header
	int tree_size = 0;
	get_tree_size(root, &tree_size);
	create_header(thrash_size, tree_size, header);
	fprintf(compressed_file, "%c%c", header[0], header[1]);

	DEBUG {
		printf("THRASH: %d TREE_SIZE: %d\n", thrash_size, tree_size);
	}
}

void compress(char *filename) {
	// Open the file
	FILE *file = fopen(filename, "r");
	if(file == NULL) {
		printf("Error! File could not be loaded successfully.\nExiting...\n");
		return;
	}

	int filename_length = strlen(filename);
	char compressed_filename[filename_length+5];
	strcpy(compressed_filename, filename);
	compressed_filename[filename_length] = '.';
	compressed_filename[filename_length+1] = 'h';
	compressed_filename[filename_length+2] = 'u';
	compressed_filename[filename_length+3] = 'f';
	compressed_filename[filename_length+4] = 'f';
	compressed_filename[filename_length+5] = '\0';
	FILE *compressed_file = fopen(compressed_filename, "r");
	
	while(compressed_file != NULL) {
		printf("Compressed file already exists.\nEnter a new file name (including extension):\n");
		char new_filename[255];
		scanf("%s", new_filename);
		new_filename[strlen(new_filename)] = '\0';
		compressed_file = fopen(new_filename, "r");
		strcpy(compressed_filename, new_filename);
	}
	compressed_file = fopen(compressed_filename, "w");
	printf("Compressing file...\n");

  	// Allocate the memory for our character table, create it and create a list of the frequencies, then sort it
	int *table = calloc(256, sizeof(int));
	huff_node *huffman_tree = malloc(sizeof(huff_node));
	
	create_table(file, table);
	huff_node *head = create_list(table);
	sort_frequency(&head);
	build_huffman_tree(&huffman_tree, head);

  // We go back to the beginning of the file and start the compression process.
	rewind(file);
	
  DEBUG {
		while(head != NULL) {
		    printf("BYTE: %c FREQ: %d\n", *((byte *)head->value), head->freq);
		    head = head->next;
		}
		print_tree(huffman_tree);
		puts("");
	}

	// Create a new file, insert header (tree size, tree preorder) and start compressing using the tree
	create_compressed_file(compressed_file, file, huffman_tree);
	
	fclose(file);
	fclose(compressed_file);
	
	printf("Done.\n");
}
#endif