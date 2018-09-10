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

/* Merge left and right lists */
fnode_l *merge(struct fnode_l *l, struct fnode_l *r){
	struct fnode_l *sorty = NULL;

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
void split_n_two(struct fnode_l *h_list, struct fnode_l **left_ref, struct fnode_l **right_ref){
	struct fnode_l *righto;
	struct fnode_l *lefty;

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
void sort_frequency_list(struct fnode_l **head_ref){
	struct fnode_l *head = *head_ref;
	struct fnode_l *l;
	struct fnode_l *r;
	if ((head == NULL) || (head->next == NULL)){
		return;
	}
	split_n_two(head, &l, &r);

	sort_frequency_list(&l);
	sort_frequency_list(&r);
	*head_ref = merge(l, r);
}



void create_frequency_list(FILE *file, fnode_l **frequency_list) {
	char *current_byte = malloc(sizeof(char *));

	// fread will read one byte at a time from the "file" variable and store it in the "current_byte" variable.
	while(fread(current_byte, 1, 1, file)) {
		// Another variable (byte_to_add) that receives the same value as current_byte is necessary, since the value of current_byte will change in every iteration
		char* byte_to_add = malloc(sizeof(char *));
		*byte_to_add = *current_byte;
		add_freq(frequency_list, (void *)byte_to_add);
	}
}

#endif
