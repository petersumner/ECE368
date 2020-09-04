#ifndef __huff_h_
#define __huff_h_

#include <stdlib.h>
#include <stdio.h>

#define OK          (  0 )
#define ERROR       ( -1 )

// Structure for nodes in the tree
struct BSTNode {
    char data;
    unsigned freq;
    struct BSTNode* left;
    struct BSTNode* right;
};

// Structure for the 'forest' of tree nodes
struct Heap {
    int size;
    struct BSTNode** array;
};

// Struture for a linked list node
struct ListNode {
    char letter;
    char* code;
    int code_size;
    struct ListNode* next;
};

// Structure for the linked list header
struct ListHead {
    struct ListNode* head;
};

#endif // _huff_h_