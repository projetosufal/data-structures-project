#ifndef _COMPRESS_H_
#define _COMPRESS_H_
#include "hufflist.h"
#include "hufftree.h"

// Function to create a table with the frequencies of each byte of the file as the value.
int *create_table(FILE* file, int* table) {
	int *current_byte = malloc(sizeof(char *));

	while(fscanf(file, "%c", current_byte) != EOF) {
		table[*current_byte] += 1;
	}
}

void compress(FILE *file) {
	// Allocate the memory for our character table, create it and create a list of the frequencies.
	int *table = calloc(256, sizeof(int));
	create_table(file, table);
	huff_node *head = create_list(table);

	// DEBUG LOOP
	int i;
	while(head != NULL) {
		printf("BYTE: %c FREQ: %d\n", *((char *)head->value), head->freq);
		head = head->next;
	}
}

/* Merge left and right lists */
huff_node *merge(struct huff_node *l, struct huff_node *r){
	struct huff_node *sorty = NULL;

	if(l == NULL){
		return(r);
	}
	else if(r==NULL){
		return(l);
	}

	if(l->freq <= r->freq){
		sorty = l;
		sorty->next = merge(l->next, r);
	}
	else{
		sorty = r;
		sorty->next = merge(l, r->next);
	} 
	return(sorty);
} 

/* keep splitting the list into left/right */
void split_n_two(struct huff_node *h_list, struct huff_node **left_ref, struct huff_node **right_ref){
	struct huff_node *righto;
	struct huff_node *lefty;

	lefty = h_list;
	righto = h_list->next;

	while(righto!=NULL){
		righto = righto->next;
		if (righto!=NULL){
			lefty = lefty->next;
			righto = righto->next;
		} 
	} 
	*left_ref = h_list;
	*right_ref = lefty->next;
	lefty->next = NULL;
}

/* Sort the frequency with the good 'ol merge sort */
void sort_frequency(struct huff_node **head_ref){
	struct huff_node *head = *head_ref;
	struct huff_node *l;
	struct huff_node *r;
	if ((head == NULL) || (head->next == NULL)){
		return;
	}
	split_n_two(head, &l, &r);

	sort_frequency(&l);
	sort_frequency(&r);
	*head_ref = merge(l, r);
}
#endif