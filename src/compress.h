#ifndef _COMPRESS_H_
#define _COMPRESS_H_
#include "hufflist.h"
#include "hufftree.h"

#define DEBUG if(1)

/* Function to create a table with the frequencies of each byte of the file as the value. */
void create_table(FILE* file, int* table) {
	unsigned char *current_byte = malloc(sizeof(char));

	while(fscanf(file, "%c", current_byte) != EOF) {
		table[*current_byte] += 1;
	}
}

void compress(FILE *file) {
  /* Allocate the memory for our character table, create it and create a list of the frequencies, then sort it */
	int *table = calloc(256, sizeof(int));
	huff_node* huffman_tree = malloc(sizeof(huff_node));
	create_table(file, table);
	huff_node *head = create_list(table);
	sort_frequency(&head);
	build_huffman_tree(&huffman_tree, head);
	print_tree(huffman_tree, 0);
	puts("");

	DEBUG {
	  while(head != NULL) {
	    printf("BYTE: %c FREQ: %d\n", *((char *)head->value), head->freq);
	    head = head->next;
	  }
	}
}
#endif
