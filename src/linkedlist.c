#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

// Every specific function and it's functionality is documented in linkedlist.h

node_l* create_node_l(void* item, node_l* next) {
	node_l* new_node = malloc(sizeof(node_l));
	new_node->value = item;
	new_node->next = next;
	return new_node;
}

fnode_l* create_fnode_l(void* item, fnode_l* next) {
	fnode_l* new_node = malloc(sizeof(fnode_l));
	new_node->value = item;
	new_node->freq = 1;
	new_node->next = next;
	return new_node;
}

void add_to_head(node_l** head, void* item) {
	*head = create_node_l(item, *head);
}

void add_freq(fnode_l** head, void* item) {

	// if the list is empty, assign a new node as head of the list.
	if(*head == NULL) {
		*head = create_fnode_l(item, *head);
		return;
	}

	// iterate through the list, and if a node of same value already exists, update the frequency, otherwise, add the new node to the end of
	// the list
	fnode_l* current = *head;
	while(current != NULL) {
		// When the current value is equal to the value we want to add:
		if(*((int *)current->value) == *((int*)item)) {
			current->freq += 1;
			return;
		}
		// When we reach the end of the list.
		if(current->next == NULL) {
			if(*((int *)current->value) == *((int*)item)) {
				current->freq += 1;
				return;
			} 
			else {
				current->next = create_fnode_l(item, NULL);
				return;
			}
		}
		current = current->next;
	}
}

void main(void) {
	fnode_l* list = NULL;
	int a = 12, b = 12, c = 12, d = 12;
	add_freq(&list, (void*)&a);
	add_freq(&list, (void*)&b);
	add_freq(&list, (void*)&c);
	add_freq(&list, (void*)&d);

	if(list == NULL) {
		printf("deuruim");
	}

	while(list != NULL) {
		printf("NODE: %d FREQ: %d\n", *((int *)list->value), list->freq);
		list = list->next;
	}
	
	exit(0);
}