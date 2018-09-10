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

// Function to add a node to a list or, if there is already a node with the same value, update the frequency of said node. 
void frequency_node(huff_node **head, void *item) {

	// if the list is empty, assign a new node as head of the list.
	if(*head == NULL) {
		*head = create_huff_node(item, 1, NULL, NULL, NULL);
		return;
	}

	// iterate through the list, and if a node of same value already exists, update the frequency, otherwise, add the new node to the end of the list
	huff_node *current = *head;
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
				current->next = create_huff_node(item, 1, NULL, NULL, NULL);
				return;
			}
		}
		current = current->next;
	}
}

// void inserir_ordenado(huff_node **head, huff_node *new_root) {
// 	huff_node *aux = NULL;

	// aux = *head;
	// while (aux->next != NULL && aux->next->freq < new_root->freq) {
	// 	aux = aux->next;
	// }

// 	huff_node *new_node = NULL;
// 	new_node = (huff_node *) malloc(sizeof(huff_node));
// 	new_node->item = item;
// 	new_node->next = aux->next;
// 	aux->next = new_node;
// }

void build_huffman_tree(huff_node **huffman_tree, huff_node *frequency_list) {

	if (frequency_list == NULL || frequency_list->next == NULL) {
		*huffman_tree = frequency_list;
		return;
	}

	printf("head da lista %p valor: %c e frequencia: %d\n", frequency_list, *((char *)frequency_list->value), frequency_list->freq);
	// printf("head da lista %p tem frequencia: %d\n", frequency_list, frequency_list->freq);

	huff_node *smallest_node = frequency_list;
	huff_node *smallest_node2 = frequency_list->next;
	printf("menores %p valor %c e %p valor %c\n", smallest_node, (char)smallest_node->value, smallest_node2, (char)smallest_node2->value);

	// remove the two nodes from the list, jumping the head to the 3rd item
	frequency_list = smallest_node2->next;

	// create a new node, sum the two nodes frequencies, point left to the smallest, right to the largest and put it into the list
	huff_node *new_node = create_huff_node("@", smallest_node->freq + smallest_node2->freq, frequency_list, smallest_node, smallest_node2);

	frequency_list = new_node; // aqui insere desordenado, mas precisa inserir ordendo

	// find the place to insert ordered the new node into the list
	// huff_node *aux_node = frequency_list;
	// while (aux_node->next != NULL && aux_node->next->freq < new_node->freq) {
	// 	aux_node = aux_node->next;
	// }

	// if (aux_node == NULL) {
	// 	frequency_list = new_node;
	// 	printf("Achei freq_list NULL\n");
	// } else {
	// 	while (aux_node->next != NULL && aux_node->next->freq < new_node->freq) {
	// 		aux_node = aux_node->next;
	// 	}
	// }

	// // 
	// new_node->next = aux_node->next;
	// aux_node->next = new_node;

	// sort_frequency_list(frequency_list);

	build_huffman_tree(huffman_tree, frequency_list);
}

void print_tree(huff_node *huffman_tree, int nivel) {

	if (huffman_tree == NULL) {
		printf(" () ");
		return;
	} else {
		printf(" ( %c ", (int)huffman_tree->value);
	}

	print_tree(huffman_tree->left, ++nivel);
	print_tree(huffman_tree->right, ++nivel);

	printf(") ");
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
void sort_frequency_list(struct huff_node **head_ref){
	struct huff_node *head = *head_ref;
	struct huff_node *l;
	struct huff_node *r;
	if ((head == NULL) || (head->next == NULL)){
		return;
	}
	split_n_two(head, &l, &r);

	sort_frequency_list(&l);
	sort_frequency_list(&r);
	*head_ref = merge(l, r);
}



void create_frequency_list(FILE *file, huff_node **frequency_list) {
	char *current_byte = malloc(sizeof(char *));

	// fread will read one byte at a time from the "file" variable and store it in the "current_byte" variable.
	while(fread(current_byte, 1, 1, file)) {
		// Another variable (byte_to_add) that receives the same value as current_byte is necessary, since the value of current_byte will change in every iteration
		char* byte_to_add = malloc(sizeof(char *));
		*byte_to_add = *current_byte;
		frequency_node(frequency_list, (void *)byte_to_add);
	}
}

#endif
