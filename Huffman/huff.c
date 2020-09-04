#include "huff.h"

char* Read_Code(char* filename, int* len, int* num);
void Get_Data(char* code, char* data, int* freq, int len, int num);
void Get_Freq(char* code, char* data, int* freq, int len, int num);
struct BSTNode* Build_Tree(char* data, int* freq, int size);
struct Heap* Build_Heap(char* data, int* freq, int size);
struct BSTNode* Remove_Node(struct Heap* heap);
void Mod_Heap(struct Heap* heap, int idx);
void Assign_Codes(struct BSTNode* root, int* arr, int top, FILE* fp, struct ListHead* list, int num);
char* Get_New_File(char* filename, char* append);
void Write_File(FILE* fp, struct ListHead* list, char* code, int len);
void Write_EOF(FILE* fp);

int main(int argc, char *argv[]) { 
    int len = 0;
    int num = 0;
    char* code = Read_Code(argv[1], &len, &num);
    char* data = malloc((num)*sizeof(*data));
    int* freq = malloc((num)*sizeof(*freq));
    Get_Data(code, data, freq, len, num);

    struct BSTNode* root = Build_Tree(data, freq, num);
    char* new_file = Get_New_File(argv[1], ".huff");
    int array[100];

    struct ListHead* list = malloc(sizeof(*list));
    list->head = NULL;
    FILE* fp = fopen(new_file, "w");
    Assign_Codes(root, array, 0, fp, list, num); 

    char c = data[0];
    fwrite(&c, sizeof(char), 1, fp);
    Write_File(fp, list, code, len);
    Write_EOF(fp);
    return EXIT_SUCCESS; 
}

// Function that reads the input file and stores it to a string.
// Also updates variables with the file character length and the
// number of unique characters.
char* Read_Code(char* filename, int* len, int* num) {
    FILE* fp = fopen(filename, "r");
    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* code = malloc(size*sizeof(*code));
    int* temp = malloc(256*sizeof(*temp));
    int i;
    for(i = 0; i < 255; i++) {
        temp[i] = 0;
    }
    char c;
    i = 0;
    int file_idx = 0;
    while(file_idx < size) {
        c = fgetc(fp);
        code[i++] = c;
        if(temp[(int)c + 128] == 0) {
            (*num)++;
        }
        temp[(int)c + 128] += 1;
        (*len)++;
        file_idx++;
    }
    return code;
}

// Function that fills two arrays with the characters found in the 
// input file and their corresponding frequencies.
void Get_Data(char* code, char* data, int* freq, int len, int num) {
    int i;
    int idx = 0;
    int* temp = malloc(256*sizeof(*temp));
    for(i = 0; i < 255; i++) {
        temp[i] = 0;
    }
    for(i = 0; i < len; i++) {
        if(temp[(int)(code[i]) + 128] == 0) {
            data[idx++] = code[i];
        }
        temp[(int)(code[i]) + 128] += 1;
    }
    idx = 0;
    for(i = 0; i < num; i++) {
        freq[i] = temp[(int)data[i] + 128];
    }
}

// Function that generates the new .huff file name.
char* Get_New_File(char* filename, char* append) {
    int i;
    i = 0;
    int len1 = 0;
    while(filename[i] != '\0') {
        len1++;
        i++;
    }
    i = 0;
    int len2 = 0;
    while(append[i] != '\0') {
        len2++;
        i++;
    }
    char* new_file = malloc(len1 + len2 + 1);
    for(i = 0; i < len1; i++) {
        new_file[i] = filename[i];
    }
    for(i = len1; i < len2 + len1; i++) {
        new_file[i] = append[i - len1];
    }
    new_file[i] = '\0';
    return new_file;
}

// Function that builds the Huffman tree using the arrays of 
// characters and frequencies.
struct BSTNode* Build_Tree(char* data, int* freq, int size) { 
    struct Heap* heap = Build_Heap(data, freq, size); 
    while (heap->size != 1) {
        struct BSTNode* left = Remove_Node(heap);
        struct BSTNode* right = Remove_Node(heap);

        struct BSTNode* top = malloc(sizeof(*top));
        top->data = '!';
        top->freq = left->freq + right->freq;
        top->left = left;
        top->right = right;

        int i = heap->size;
        heap->size += 1;
        int idx = (i-1) / 2;
        while(i && (heap->array[idx]->freq > top->freq)) {
            heap->array[i] = heap->array[idx];
            i = idx;
            idx = (i-1) / 2;
        }
        heap->array[i] = top;
    }
    struct BSTNode* root = Remove_Node(heap);
    return root;
}

// Function that creates a heap 'forest' of tree nodes for each
// character in the file.
struct Heap* Build_Heap(char* data, int* freq, int size) { 
    struct Heap* heap = malloc(sizeof(*heap)); 
    heap->size = size;
    heap->array = malloc(size*sizeof(*heap->array));
    int i;
    for (i = 0; i < size; i++) {
        struct BSTNode* node = malloc(sizeof(*node)); 
        node->data = data[i];
        node->freq = freq[i];
        node->right = NULL;
        node->left = NULL;
        heap->array[i] = node;
    }
    for (i = size / 2; i >= 0; i--) { 
        Mod_Heap(heap, i); 
    }
    return heap; 
} 

// Function that removes a node from the heap.
struct BSTNode* Remove_Node(struct Heap* heap) { 
    struct BSTNode* temp = heap->array[0]; 
    heap->size -= 1; 
    heap->array[0] = heap->array[heap->size]; 
    Mod_Heap(heap, 0); 
    return temp; 
}

// Function to sort the heap so it can be used to create a binary
// tree to use for the Huffman tree
void Mod_Heap(struct Heap* heap, int idx) { 
    int x = idx; 
    int left = 1 + 2*idx; 
    int right = left + 1; 
    if (heap->size > left) {
        if(heap->array[left]->freq < heap->array[x]->freq) {
            x = left;
        }
    }
    if (heap->size > right) {
        if(heap->array[right]->freq < heap->array[x]->freq) {
            x = right;
        }
    }
    if (x != idx) { 
        struct BSTNode* temp = heap->array[x];
        heap->array[x] = heap->array[idx];
        heap->array[idx] = temp;
        Mod_Heap(heap, x); 
    } 
} 
  
// Function that recursively assigns codes for the characters in the
// Huffman tree. Also writes the header of the new file ass codes
// are assigned.
void Assign_Codes(struct BSTNode* root, int* arr, int top, FILE* fp, struct ListHead* list, int num) {
    if(num != -1) {
        fwrite(&num, sizeof(int), 1, fp);
    }
    if (root->left) { 
        arr[top] = 0; 
        Assign_Codes(root->left, arr, top + 1, fp, list, -1); 
    } 
    if (root->right) { 
        arr[top] = 1; 
        Assign_Codes(root->right, arr, top + 1, fp, list, -1); 
    }
    if (!(root->left) && !(root->right)) { 
        struct ListNode* node = malloc(sizeof(*node));
        node->letter = root->data;
        node->code = malloc(top*sizeof(*node->code));
        node->next = NULL;
        if(list->head == NULL) {
            list->head = node;
        }
        else{
            struct ListNode* head = list->head;
            while(head->next != NULL) {
                head = head->next;
            }
            head->next = node;
        }
        int r = root->data;
        fwrite(&r, sizeof(char), 1, fp);
        int i;
        node->code_size = top;
        for (i = 0; i < top; i++) {
            node->code[i] = arr[i];
            char c = arr[i] + '0';
            fwrite(&c, sizeof(char), 1, fp);
        }

        char end = ':';
        fwrite(&end, sizeof(char), 1, fp);
    }
} 

// Function that reads through the file characters and writes their
// corresponding code to the new file.
void Write_File(FILE* fp, struct ListHead* list, char* code, int len) {
    int i;
    for(i = 0; i < len; i++) {
        char c = code[i];
        struct ListNode* head = list->head;
        while(head->letter != c && head->next != NULL) {
            head = head->next;
        }
        int j;
        for(j = 0; j < head->code_size; j++) {
            char c = head->code[j] + '0';
            fwrite(&c, sizeof(char), 1, fp);
        }
    }
}

// Function to write extra bits to the end of the file.
void Write_EOF(FILE* fp) {
    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    int bits = size % 8;
    int i;
    for(i = 0; i < bits; i++) {
        fputc(0, fp);
    }
}