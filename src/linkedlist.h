#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

typedef struct node_l {
	void* value;
	struct node_l* next;
} node_l;

typedef struct fnode_l {
	void* value;
	int freq;
	struct fnode_l* next;
} fnode_l;

// Function that allocates the memory and returns the address of a new list node.
node_l* create_node_l(void* item, node_l* next);

// Function that allocates the memory and returns the address of a new frequency list node.
fnode_l* create_fnode_l(void* item, fnode_l* next);

// Function to add a node to the head of a certain linked list.
void add_to_head(node_l** head, void* item);

// Function to add a node to a list or, if there is already a node with the same value, update the frequency of said node. 
void add_freq(fnode_l** head, void* item);

#endif