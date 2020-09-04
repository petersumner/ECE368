#include <stdlib.h>
#include <stdio.h>

struct Node {
    int value;
    struct Node* next;
};

struct Heap {
    int size;
    struct Node** array;
};

int string_to_int(char* string, int start, int end) {
    int num = 0;
    int i;
    int j;
    for(i=start; i<end; i++) {
        int x = 1;
        for(j=start; j<i-1; j++) {
            x *= 10;
        }
        num += string[i] * x;
    }
    return num;
}

void split_line(char* line, int* data) {
    int i = 0;
    while(line[i] != ' ')
        i++;
    int j = i+1;
    while(line[j] < 58 && line[j] > 47)
        j++;
    data[0] = string_to_int(line, 0, i);
    data[1] = string_to_int(line, i+1, j);
}

void read_map(char* file) {
    FILE* fp = fopen(file, "r");
    char* line = NULL;
    size_t len = 0;

    if(getline(&line, &len, fp) == -1)
        return;
    int data[2];
    split_line(line, data);
    struct Heap* heap = malloc(sizeof(*heap));
    heap->size = data[0] - 48;
    heap->array = malloc(sizeof(*heap->array)*(data[0]-48));
    int edges = data[1]-48;
    int i;
    for(i=0; i<(data[0]-48); i++){
        getline(&line, &len, fp);
        struct Node* node = malloc(sizeof(*node));
        node->value = i+1;
        node->next = NULL;
        heap->array[i] = node;
    }
    for(i=0; i<edges; i++){
        getline(&line, &len, fp);
        split_line(line, data);
        struct Node* node = malloc(sizeof(*node));
        node = heap->array[data[0]-48];
        while(node->next != NULL) {
            node = node->next;
        }
        struct Node* newNode = malloc(sizeof(*newNode));
        newNode->value = data[1]-48;
        newNode->next = NULL;
        node->next = newNode;
    }
    for(i=0; i<heap->size; i++) {
        printf("%d: ", i);
        struct Node* node = malloc(sizeof(*node));
        node = heap->array[i];
        while (node->next != NULL){
            printf("%d ", node->next->value);
            node = node->next;
        }
        printf("\n");
    }
    /*

    int nodes = 0;
    int edges = 0;
    int line_num = 0;
    while(getline(&line, &len, fp) != -1) {
        if(nodes == 0 && edges == 0) {
            int i = 0;
            while(line[i] != ' ') {
                i++;
            }
            printf("%d\n", i);
            nodes = line[i-1] - 48;
            edges = line[i+1] - 48;
            printf("%d %d\n", nodes, edges);
            struct Heap* heap = malloc(sizeof(*heap));
            heap->size = nodes;
            heap->array = malloc(sizeof(*heap->array)*edges);
        }
        if(nodes != 0 && line_num > nodes) {
            printf("%s", line);
            int i = 0;
            while(line[i] != ' ') {
                i++;
            }


        }
        len = 0;
        line_num++;
    }
    */
}
int main(int argc, char *argv[]) { 
    read_map(argv[1]);
    return EXIT_SUCCESS;
}