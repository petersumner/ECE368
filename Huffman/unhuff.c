#include "huff.h"

char* Read_Data(char* filename, int* num_nodes);
struct ListHead* Read_Header(char* data, int* offset, int num_nodes);
void Write_Huff(char* data, struct ListHead* header, int offset, FILE* fp);
char* Get_New_File(char* filename, char* append);

int main(int argc, char *argv[]){
    int num_nodes = 0;
    char* data = Read_Data(argv[1], &num_nodes);
    int offset = 0;
    struct ListHead* header = Read_Header(data, &offset, num_nodes);
    offset++;
    char* new_file = Get_New_File(argv[1], ".unhuff");
    FILE* fp = fopen(new_file, "w");
    Write_Huff(data, header, offset, fp);
    free(data);
    free(header);
    return EXIT_SUCCESS;
}

char* Read_Data(char* filename, int* num_nodes) {
    FILE* fp = fopen(filename, "r");
    fseek(fp, 0, SEEK_END);
    size_t file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    fread(num_nodes, sizeof(int), 1, fp);
    fseek(fp, 0, SEEK_SET);
    char* data = malloc(sizeof(*data)*file_size + 1);
    fread(data, file_size, 1, fp);
    data[file_size] = '\0';
    fclose(fp);
    return data;
}

struct ListHead* Read_Header(char* data, int* offset, int num_nodes){
    struct ListHead* header = malloc(sizeof(*header));
    header->head = NULL;
    *offset = 4;
    int i = 0;
    while(i < num_nodes) {
        char code[100];
        int c_idx = 0;
        char letter = data[(*offset)++];
        while(data[*offset] != ':') {
            code[c_idx++] = data[(*offset)++];
        }
        (*offset)++;
        struct ListNode* node = malloc(sizeof(*node));
        node->letter = letter;
        node->code = malloc(c_idx*sizeof(char));
        node->code_size = c_idx;
        int j;
        for(j = 0; j < c_idx; j++) {
            node->code[j] = code[j];
        }
        node->next = NULL;
        if(header->head == NULL) {
            header->head = node;
        }
        else{
            struct ListNode *head = header->head;
            while(head->next != NULL) {
                head = head->next;
            }
            head->next = node;
        }
        i++;
    }
    return header;
}

void Write_Huff(char* data, struct ListHead* header, int offset, FILE* fp){
    char code[1000];
    int idx;
    for(idx = 0; idx < 1000; idx++) {
        code[idx] = '\0';
    }
    idx = 0;
    while(data[offset] != '\0') {
        code[idx] = data[offset];
        struct ListNode* node = header->head;
        int match = 0;
        while(node != NULL) {
            if (idx + 1 == node->code_size) {
                int i;
                match = 1;
                for(i = 0; i < node->code_size; i++){
                    if(code[i] != node->code[i]) {
                        match = 0;
                    }
                }
                if(match == 1) {
                    idx = 0;
                    fwrite(&(node->letter), sizeof(char), 1, fp);
                }
            }
            node = node->next;
        }
        offset++;
        if (match == 0){
            idx++;
        }
    }
}

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