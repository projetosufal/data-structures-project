#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"
#define DEBUG 1

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