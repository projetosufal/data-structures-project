/*
Minimal huffman compression program
Written by:
Nicolas Leão, Matheus Artur, Luis Cabus and Fábio Vinícius.
UFAL
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hufflist.h"
#include "hufftree.h"
#define DEBUG 0

void compress(FILE* file) {
	huff_node *frequency_list = NULL;
	huff_node * huffman_tree = NULL;

	// Create a frequency list from the bytes in the file and sort it.
	create_frequency_list(file, &frequency_list);
	sort_frequency_list(&frequency_list);

	// Debug loop to print the bytes in hexadecimal and their frequencies.
	huff_node *aux_list = frequency_list;
	while(aux_list != NULL && DEBUG == 1) {
		printf("BYTE: %hhx / FREQ: %d\n", *((char *)aux_list->value), aux_list->freq);
		aux_list = aux_list->next;
	}
	puts("");

	// Create the file's huffman tree
	build_huffman_tree(&huffman_tree, frequency_list);

	puts("");
}

int main(int argc, char **argv) {
	/* 
	The command that will determine the operation to be executed is passed as the first argument (argv[1]).
	the available command/operation touples are: (-c, compress), (-e, extract) and (-h, help).
	The file that will be used is passed as the second argument (argv[2]) when the program is executed. 

	An example of the usage is: ./program -c picture.jpg, that will compress the file "picture.jpg"
	The file specified will be stored in the variable "file", as read-only, to ensure that the original data is not
	altered in the compression process. 
	*/
	if(argv[1] == NULL || strcmp(argv[1], "-h") == 0) {
		printf("\nThe correct syntax to use the program is: ./program <command> <file>\n");
		printf("Available commands are:\n");
		printf("-c, Compress the specified file.\n");
		printf("-e, Extract the specified file.\n");
		printf("Example usage: ./program -c myfile.png\n\n");
	}
	else if(strcmp(argv[1], "-c") == 0) {
		FILE *file = NULL;
		if(argv[2] != NULL) {
			file = fopen(argv[2], "r");
		}
		if(file == NULL) {
			printf("Error! File could not be loaded successfully.\nExiting...\n");
			return 1;
		}
		compress(file);
		fclose(file);
	} 
	else if(strcmp(argv[1], "-e") == 0) {
		FILE *file = NULL;
		if(argv[2] != NULL) {
			file = fopen(argv[2], "r");
		}
		if(file == NULL) {
			printf("Error! File could not be loaded successfully.\nExiting...\n");
			return 1;
		}
		char *current_byte = malloc(sizeof(char *));
		while(fread(current_byte, 1, 1, file)) {
			//TODO: extraction algorithm.
		}
		fclose(file);
	}
	else {
		printf("Invalid command specified, use ./program -h to see a list of available commands.\n");
	}
	
	return 0;
}
