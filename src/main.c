/*
Minimal huffman compression program
Written by:
Nicolas Leão, Matheus Artur, Luis Cabus and Fábio Vinícius.
UFAL
*/
#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

void main(int argc, char** argv) {
	// The command that will determine the operation to be executed is passed as the first argument (argv[1]).
	// the available command/operation touples are: (-c, compress) and (-e, extract). 
	// The file that will be used is passed as the second argument (argv[2]) when the program is executed.
	
	// An example of the usage is: ./huffman -c picture.jpg, that will compress the file "picture.jpg"

	// The file specified will be stored in the variable "file", as read-only to ensure that the original data is not
	// altered in the compression process.
	FILE* file = fopen(argv[2], "r");
	if(file == NULL) {
		printf("Error! File could not be loaded successfully.\nExiting...\n");
		exit(1);
	}

	if(strcmp(argv[1], "-c") == 0) {
		void* current_byte;
		//fread will read one byte at a time from the "file" variable and store it in the "current_byte" variable.
		while(fread(current_byte, 1, 1, file) != EOF) {
			//TODO: compression algorithm.
			//magic happens here.
		}
	} 
	else if(strcmp(argv[1], "-e") == 0) {
		void* current_byte;
		//fread will read one byte at a time from the "file" variable and store it in the "current_byte" variable.
		while(fread(current_byte, 1, 1, file) != EOF) {
			//TODO: extraction algorithm.
			//magic happens here.
		}
	}
	fclose(file);
	exit(0);
}
