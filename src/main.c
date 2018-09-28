#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"
#include "extract.h"

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
		if(argv[2] == NULL) {
			printf("Error! No file specified.\nExiting...\n");
			return 1;
		}
		compress(argv[2]);
	} 
	else if(strcmp(argv[1], "-e") == 0) {
		if(argv[2] == NULL) {
			printf("Error! No file specified.\nExiting...\n");
			return 1;
		}
		extract(argv[2]);
	}
	else {
		printf("Invalid command specified, use ./program -h to see a list of available commands.\n");
	}
	
	return 0;
}
