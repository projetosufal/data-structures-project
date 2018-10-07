#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

typedef struct node_srtruct { 
	int data; 
	int priority; 
	struct node_srtruct *next; 
} node_srtruct; 

node_srtruct *newNode(int iData, int iPriority) { 
	node_srtruct * temp = (node_srtruct *)malloc(sizeof(node_srtruct)); 
	temp->data = iData; 
	temp->priority = iPriority; 
	temp->next = NULL; 
	return temp; 
}

int push(node_srtruct **head, int data, int priority, int i) { 
	
	node_srtruct *aux = *head;
	node_srtruct *temp = newNode(data, priority);

	if (*head == NULL) {
		*head = temp;
	} else if ((*head)->priority > priority) { // if new item has higher priority
		temp->next = *head; 
		*head = temp; 
	} 
	else { // Find the position to insert new node_srtruct 
		while (aux->next != NULL && aux->next->priority < priority) { 
			aux = aux->next; 
			++i;
		} 
		// Insert at the end of the list or at required position (whichever comes first)
		temp->next = aux->next; 
		aux->next = temp; 
	} 

	return i;
} 

void parsefile(char *origin_name, char *destination_name) {
	int iterates = 0, acumulado = 0; 

	node_srtruct *pQueue = NULL;

	FILE *origin_file;
	origin_file = fopen(origin_name, "r");

	FILE *destination_file;
	destination_file = fopen(destination_name, "w");

	int item, priority, i = 0;
	while (fscanf(origin_file, "%d %d", &item, &priority) != EOF) {
		// printf("data %d prioridade %d\n", item, priority);
		iterates = push(&pQueue, item, priority, 1);
		acumulado += iterates;
		fprintf(destination_file, "%d %d %d\n", i, iterates, acumulado);
		++i;
	}
	printf("Total de inserções: %d \nTotal de iterações: %d\n", i, acumulado);
}


int main(int argc, char *argv[]) {

	if(argv[1] == NULL ) {
		printf("\nThe correct syntax to use this program is: ./program <command> <file>\n");
		printf("Example usage: ./program input_file.txt\n\n");
	} else {
		char savename[256];
		printf("Com que nome deseja exportar o log? (inserir formato .txt)\n");
		scanf("%s", savename);
		parsefile(argv[1], savename);
	}

	return 0;
}