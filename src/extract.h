#ifndef _EXTRACT_H_
#define _EXTRACT_H_
#include <math.h>
#include "hufflist.h"
#include "hufftree.h"
#include <stdbool.h>

bool get_bit(unsigned char c, int i) {
	unsigned char aux = 1 << i;
	return aux & c;
}

void extract(FILE* file) {
	/* The two first bytes of the file provides us the information needed to build the tree that will be used */
	unsigned char header[2];
	int thrash = 0, tree_size = 0;
	fscanf(file, "%c%c", &header[0], &header[1]);

	// Three fors to organize the reading of the header values.
	for(int i = 5; i < 8; ++i) {
	  thrash += get_bit(header[0], i)*pow(2, i);
	}
	for(int i = 0; i < 8; ++i) {
		tree_size += get_bit(header[1], i)*pow(2, i);
	}
	for(int i = 0; i < 5; ++i) {
		tree_size += get_bit(header[0], i)*pow(2, 8+i);
	}

	DEBUG {
		printf("THRASH: %d TREE_SIZE: %d\n", thrash, tree_size);
	}
}

#endif
