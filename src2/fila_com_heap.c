#include <stdio.h>

int heapsize = 0;

typedef struct struct_queue {
	int data;
	int priority;
} struct_queue;

#define SIZE 99999
struct_queue ArrHeap[SIZE];

void init_heap() {
	for(int i = 0; i < SIZE; i++) {
		ArrHeap[i].priority = 0;
		ArrHeap[i].data = 0;
	}
}

void printheap() {
	for(int i = 0; i < heapsize; i++) {
		printf("data %d prioridade %d \n", ArrHeap[i].data, ArrHeap[i].priority);
	}
}



int parent(int i) {
	return ((i - 1) / 2);
}

void swap(int *x, int *y) {
	int temp = *x;
	*x = *y;
	*y = temp;
}

int compare_priority(const struct_queue *a, const struct_queue *b) {
	if (a->priority < b->priority) return 1;
	if (a->priority > b->priority) return 0;

	return (a->data > b->data);
}

int insertkey(int iData, int iPriority, int j) {
    int i = heapsize++;

    ArrHeap[i].priority = iPriority;
    ArrHeap[i].data = iData;

    while (i != 0 && compare_priority(&ArrHeap[parent(i)], &ArrHeap[i])) {
        swap(&ArrHeap[i].priority, &ArrHeap[parent(i)].priority);
        swap(&ArrHeap[i].data, &ArrHeap[parent(i)].data);
        i = parent(i);
        ++j;
    }

    return j;
}

void parsefile(char *origin_file, char *destination_name) {
	int iterates = 0, acumulado = 0; 

	FILE *file_name;
	file_name = fopen(origin_file, "r");

	FILE *destination_file;
	destination_file = fopen(destination_name, "w");

	int item, priority, i = 0;
	while (fscanf(file_name, "%d %d", &item, &priority) != EOF) {
		// printf("data %d priority %d\n", item, priority);
		iterates = insertkey(item, priority, 1);
		acumulado += iterates;
		fprintf(destination_file, "%d %d %d\n", i, iterates, acumulado);
		++i;
	}
	// printheap();
	printf("Total de inserções: %d \nTotal de iterações: %d\n", i, acumulado);
}


int main(int argc, char *argv[]) {

	init_heap();

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