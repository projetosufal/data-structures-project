#ifndef _EXTRACT_H_
#define _EXTRACT_H_
#include <math.h>
#include "hufflist.h"
#include "hufftree.h"

int is_bit_set(unsigned char c, int i) {
	unsigned char aux = 1 << i;
	return aux & c;
}

void extract(FILE* file) {
	/* The two first bytes of the file provides us the information needed to build the tree that will be used */
	unsigned char header[2];
	int thrash = 0, tree_size = 0;
	fscanf(file, "%c%c", header[0], header[1]);
	for(int i = 3; i > 0; --i) {
	  thrash += is_bit_set(header[0], i)*pow(2, i);
	}
	for(int i = 8; i > 0; --i) {
	  tree_size += is_bit_set(header[1], i)*pow(2, i);
	}
	printf("%d\n", thrash);
}

#endif
