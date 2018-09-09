#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

typedef struct node_l {
	void *value;
	struct node_l *next;
} node_l;

typedef struct fnode_l {
	void *value;
	int freq;
	struct fnode_l *next;
} fnode_l;

// Function that allocates the memory and returns the address of a new list node.
node_l *create_node_l(void *item, node_l *next) {
	node_l *new_node = malloc(sizeof(node_l));
	new_node->value = item;
	new_node->next = next;
	return new_node;
}

// Function that allocates the memory and returns the address of a new frequency list node.
fnode_l *create_fnode_l(void *item, fnode_l *next) {
	fnode_l *new_node = malloc(sizeof(fnode_l));
	new_node->value = item;
	new_node->freq = 1;
	new_node->next = next;
	return new_node;
}

// Function to add a node to the head of a certain linked list.
void add_to_head(node_l **head, void *item) {
	*head = create_node_l(item, *head);
}

// Function to add a node to a list or, if there is already a node with the same value, update the frequency of said node. 
void add_freq(fnode_l **head, void *item) {

	// if the list is empty, assign a new node as head of the list.
	if(*head == NULL) {
		*head = create_fnode_l(item, *head);
		return;
	}

	// iterate through the list, and if a node of same value already exists, update the frequency, otherwise, add the new node to the end of
	// the list
	fnode_l *current = *head;
	while(current != NULL) {
		// When the current value is equal to the value we want to add:
		if(*((int *)current->value) == *((int *)item)) {
			current->freq += 1;
			return;
		}
		// When we reach the end of the list.
		if(current->next == NULL) {
			if(*((int *)current->value) == *((int *)item)) {
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

#endif