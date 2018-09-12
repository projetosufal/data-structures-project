#ifndef _HUFFLIST_H_
#define _HUFFLIST_H_

typedef struct huff_node {
	void *value;
	int freq;
	struct huff_node *next;
	struct huff_node *left;
	struct huff_node *right;
} huff_node;


/* Function that allocates the memory and returns the address of a new huff frequency list node. */
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
