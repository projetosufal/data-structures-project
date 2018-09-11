#ifndef _HUFFLIST_H_
#define _HUFFLIST_H_

typedef struct huff_node {
	void *value;
	int freq;
	struct huff_node *next;
	struct huff_node *left;
	struct huff_node *right;
} huff_node;


// Function that allocates the memory and returns the address of a new huff frequency list node.
huff_node *create_huff_node(void *item, int freq, huff_node *next, huff_node *left, huff_node *right) {
	huff_node *new_node = malloc(sizeof(huff_node));
	new_node->value = item;
	new_node->freq = freq;
	new_node->next = next;
	new_node->left = left;
	new_node->right = right;
	return new_node;
}

void add_to_head(huff_node **head, void *item, int freq) {
	*head = create_huff_node(item, freq, *head, NULL, NULL);
}

/* Function to create a "linked list" using the elements of an integer array as the frequency 
and the index converted to ascii as the value. */
huff_node* create_list(int* table) {
	int i;
	huff_node* head = NULL;
	for(i = 0; i < 256; i++) {
		if(table[i] != 0) {
			char *curr_char = malloc(sizeof(char *));
			*curr_char = (char)i;
			add_to_head(&head, (void *)curr_char, table[i]);
		}
	}
	return head;
}

// Function to add a node to a list or, if there is already a node with the same value, update the frequency of said node. 
void add_frequency(huff_node **head, void *item) {

	// if the list is empty, assign a new node as head of the list.
	if(*head == NULL) {
		*head = create_huff_node(item, 1, NULL, NULL, NULL);
		return;
	}

	// iterate through the list, and if a node of same value already exists, update the frequency, otherwise, add the new node to the end of the list
	huff_node *current = *head;
	while(current != NULL) {
		// When the current value is equal to the value we want to add:
		if(*((char *)current->value) == *((char *)item)) {
			current->freq += 1;
			return;
		}
		// When we reach the end of the list.
		if(current->next == NULL) {
			if(*((char *)current->value) == *((char *)item)) {
				current->freq += 1;
				return;
			} 
			else {
				current->next = create_huff_node(item, 1, NULL, NULL, NULL);
				return;
			}
		}
		current = current->next;
	}
}


#endif
