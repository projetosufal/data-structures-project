#ifndef _COMPRESS_H_
#define _COMPRESS_H_
#include "hufflist.h"
#include "hufftree.h"

#define DEBUG if(1)

/* Function to create a table with the frequencies of each byte of the file as the value. */
int *create_table(FILE* file, int* table) {
	int *current_byte = malloc(sizeof(char *));

	while(fscanf(file, "%c", current_byte) != EOF) {
		table[*current_byte] += 1;
	}
}

void compress(FILE *file) {
  /* Allocate the memory for our character table, create it and create a list of the frequencies, then sort it */
	int *table = calloc(256, sizeof(int));
	create_table(file, table);
	huff_node *head = create_list(table);
	sort_frequency(&head);

	DEBUG{
	  while(head != NULL) {
	    printf("BYTE: %c FREQ: %d\n", *((char *)head->value), head->freq);
	    head = head->next;
	  }
	}
}
#endif
