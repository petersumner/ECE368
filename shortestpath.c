#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

struct EdgeNode{
    int destination;
    float distance;
    struct EdgeNode* next;
};

struct Vertex{
    int x;
    int y;
    struct EdgeNode* head;
};

struct HeapNode{
    int vertex;
    float length;
};

struct Map{
    int num_vertices;
    int num_edges;
    int queries;
    int source;
    int destination;
    int visited;
    int elements;
    struct Vertex* vertices;
};

struct Path{
    float* distances;
    int* forward;
    int* position;
};

float calculateDistance(int x, int y, int p, int q){
    return sqrt( (x-p)*(x-p) + (y-q)*(y-q) );
}

void addEdgeNode(struct Vertex* v, int src, int dest){
    struct EdgeNode* newNode = malloc(sizeof(*newNode));
    newNode->destination = dest;
    newNode->distance = calculateDistance(v[src].x, v[src].y, v[dest].x, v[dest].y);
    newNode->next = NULL;

    struct EdgeNode* head = v[src].head;

    if(head == NULL)
        v[src].head = newNode;
    else if(head->next == NULL && head->destination < dest)
        v[src].head->next = newNode;
    else if(head->next == NULL && head->destination >= dest)
        newNode->next = v[src].head;
    else if(head->destination > newNode->destination){
        v[src].head = newNode;
        newNode->next = v[src].head;
    }
    else{
        struct EdgeNode* temp = v[src].head;
        while(temp->next != NULL && temp->next->destination <= newNode->destination)
            temp = temp->next;
        if(temp->next == NULL)
            return;
        else
            newNode->next = temp->next;
    }
}

struct Map* fillMap(char* file){
    FILE* fp = fopen(file, "r");
    struct Map* map = malloc(sizeof(*map));

    fscanf(fp, "%d %d", &(map->num_vertices), &(map->num_edges));
    int i;

    struct Vertex* v = malloc(sizeof(*v)*map->num_vertices);
    for(i=0; i<map->num_vertices; i++){
        v[i].x = -1;
        v[i].y = -1;
        v[i].head = NULL;
    }

    map->queries = -1;
    map->source = -1;
    map->destination = -1;
    map->visited = map->num_vertices;
    map->vertices = v;

    int hold;
    for(i=0; i<map->num_vertices; i++){
        fscanf(fp, "%d %d %d", &hold, &(v[i].x), &(v[i].y));
    }
    int src;
    int dest;
    for(i=0; i<map->num_edges; i++){
        fscanf(fp, "%d %d", &src, &dest);
        addEdgeNode(v, src, dest);
        addEdgeNode(v, dest, src);
    }

    fclose(fp);
    return map;
}

void freePath(struct Path* path){
    free(path->distances);
    free(path->forward);
    free(path);
}

struct Path* initPath(int size, int source){
    struct Path* path = malloc(sizeof(*path));
    path->distances = malloc(sizeof(*(path->distances))*size);
    path->forward = malloc(sizeof(*(path->forward))*size);
    path->position = malloc(sizeof(*(path->position))*size);
    int i;
    for(i=0; i<size; i++){
        path->forward[i] = -1;
        path->position[i] = i;
        if(i != source)
            path->distances[i] = -1;
        else
            path->distances[i] = 0;
    }
    return path;
}

void setLength(struct HeapNode** heap, struct Path* path, int idx){
    int i = path->position[idx];
    int i2 = (i-1)/2;
    heap[i]->length = path->distances[idx];
    int len1 = heap[i]->length;
    int len2 = heap[i2]->length;

    while(i>0 && len2 > len1){
        path->position[heap[i2]->vertex] = i;
        path->position[heap[i]->vertex] = i2;
        struct HeapNode* temp = heap[i];
        heap[i]->vertex = heap[i2]->vertex;
        heap[i]->length = heap[i2]->length;
        heap[i2]->vertex = temp->vertex;
        heap[i2]->length = temp->length;

        i = (i-1)/2;
        i2 = (i-1)/2;
    }
}

struct HeapNode** initHeap(struct Path* path, int size, int source){
    struct HeapNode** heap = malloc(sizeof(*heap)*size);
    int i;
    for(i=0; i<size; i++){
        struct HeapNode* newNode = malloc(sizeof(*newNode));
        newNode->length = path->distances[i];
        newNode->vertex = i;
        heap[i] = newNode;
    }
    return heap;
}

void Heapify(struct HeapNode** heap, struct Path* path, int idx, int size){
    int hold = idx;
    int idx2 = 2*idx;
    if(idx2+1 < size && heap[idx2+1]->length < heap[idx]->length)
        idx = idx2+1;
        struct HeapNode* temp = heap[idx];
        path->position[heap[idx]->vertex] = hold;
        path->position[heap[hold]->vertex] = idx;
        heap[idx] = heap[hold];
        heap[hold] = temp;
    if(idx2+2 < size && heap[idx2+2]->length < heap[idx]->length)
        idx = idx2+2;
        temp = heap[idx];
        path->position[heap[idx]->vertex] = hold;
        path->position[heap[hold]->vertex] = idx;
        heap[idx] = heap[hold];
        heap[hold] = temp;
    if(idx != hold)
        Heapify(heap, path, idx, size);
}

struct HeapNode* getMin(struct HeapNode** heap, struct Path* path, struct Map* map){
    if(map->elements != 0){
        struct HeapNode* first = heap[0];
        (map->elements)--;
        heap[0] = heap[map->elements];
        path->position[heap[0]->vertex] = 0;
        path->position[first->vertex] = map->elements;
        Heapify(heap, path, 0, map->elements);
        return first;
    }
    return NULL;
}

void printPath(struct Path* path, struct Map* map){
    if(path->distances[map->destination] != -1)
        printf("%f\n", path->distances[map->destination]);
    else
        printf("INF\n");
        printf("%d %d\n", map->source, map->destination);
}

void shortestPath(struct Map* map, FILE* fp){
    fscanf(fp, "%d %d", &(map->source), &(map->destination));

    struct Path* path = initPath(map->num_vertices, map->source);
    path->forward[map->source] = 0;

    struct HeapNode** heap = initHeap(path, map->num_vertices, map->source);
    setLength(heap, path, map->source);

    while(map->visited < map->num_vertices && path->distances[map->destination] == -1){
        struct HeapNode* node = getMin(heap, path, map);
        int edgeIdx = node->vertex;
        struct EdgeNode* edge = map->vertices[edgeIdx].head;
        while(edge != NULL){
            int elements = path->position[edge->destination];
            if(map->visited > elements && path->distances[edgeIdx] != -1){
                int pathDist = path->distances[edgeIdx];
                if(edge->distance + pathDist){
                    int edgeDest = edge->destination;
                    int edgeDist = edge->distance;
                    path->distances[edgeDest] = path->distances[edgeIdx] + edgeDist;
                    setLength(heap, path, edgeDest);
                }
            }
            edge = edge->next;
        }
    }

    printPath(path, map);

    free(heap);
    freePath(path);
}

void findPaths(struct Map* map, char* file){
    FILE* fp = fopen(file, "r");
    fscanf(fp, "%d", &(map->queries));
    int i;
    for(i=0; i<map->queries; i++){
        shortestPath(map, fp);
    }
    fclose(fp);
}

int main(int argc, char* argv[]){
    struct Map* map = fillMap(argv[1]);
    findPaths(map, argv[2]);
    free(map);
    return EXIT_SUCCESS;
}