#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include <string.h>
#include <stdio.h>

typedef struct HuffmanNode {
    unsigned char value;
    struct HuffmanNode *left,
                        *right,
                        *parent;
} HuffmanNode, *PHuffmanNode;

typedef PHuffmanNode T;

#include "heap.h"


PHuffmanNode initNode(unsigned char value) {

    PHuffmanNode newNode = (PHuffmanNode) malloc(sizeof(HuffmanNode));
    newNode->value = value;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;
    return newNode;
}

void computeFreqs(char *text, int freqs[256]) {
    memset(freqs, 0, 256 * sizeof(int)); // Initialize all elements to 0
    
    for (int i = 0; text[i] != '\0'; i++) {
        freqs[(unsigned char) text[i]]++; // Increment the frequency of the corresponding character
    }
}

PHuffmanNode makeTree(int freqs[256]) {
    PHeap heap = makeHeap(256);

    // Fill the heap with nodes, one for each character with a frequency > 0. 
    for (int i = 0; i < 256; i++) {
        if (freqs[i] > 0) {
            PHuffmanNode node = initNode(i);
            node->parent = NULL;
            insert(heap, node, freqs[i]);
        }
    }

    // Now construct the Huffman tree by combining the nodes with the smallest frequencies. 
    while (heap->size > 1) {

        PHeapNode min1 = removeMin(heap);
        PHeapNode min2 = removeMin(heap);
        PHuffmanNode newNode = initNode(0);

        newNode->left = min1->elem;
        min1->elem->parent = newNode;

        newNode->right = min2->elem;
        min2->elem->parent = newNode;

        insert(heap, newNode, min1->prior + min2->prior);

        // No need to free min1 and min2 here, they are still used in the tree. 
        free(min1);
        free(min2);
    }

    // Now there's only one node left in the heap - the root of the Huffman tree. 
    PHeapNode root = removeMin(heap);
    PHuffmanNode huffRoot = root->elem;
    free(root);

    // Free the heap.
    freeHeap(heap);

    return huffRoot;
}


void makeCodesHelper(PHuffmanNode root, char *code, char **allCodes) {
    if (root == NULL) {
        return;
    }
    
    if (root->left == NULL && root->right == NULL) {
        allCodes[root->value] = strdup(code);
        return;
    }
    
    int len = strlen(code);
    char *leftCode = (char *) malloc(len + 2);
    char *rightCode = (char *) malloc(len + 2);
    sprintf(leftCode, "%s%c", code, '0');
    sprintf(rightCode, "%s%c", code, '1');
    
    makeCodesHelper(root->left, leftCode, allCodes);
    makeCodesHelper(root->right, rightCode, allCodes);
    
    free(leftCode);
    free(rightCode);
}

void makeCodes(PHuffmanNode root, char **allCodes) {
    char *code = (char *) malloc(sizeof(char));
    code[0] = '\0';
    makeCodesHelper(root, code, allCodes);
    free(code);
}



char *compress(char *textToEncode, char **allCodes) {

    int len = strlen(textToEncode);

    char *compressed = (char *) malloc(sizeof(char) * (len * 10)); // allocate some memory for the compressed string

    compressed[0] = '\0'; // initialize the compressed string to an empty string

    // traverse the input text and concatenate their corresponding Huffman codes
    for (int i = 0; i < len; i++) {
        strcat(compressed, allCodes[(int) textToEncode[i]]); // concatenate the Huffman code for the current character
    }

    return compressed;
}

char *decompress(char *textToDecode, PHuffmanNode root) {
    PHuffmanNode current = root;
    
    char *decodedText = (char*)malloc(1);
    int decodedTextCapacity = 1;
    int decodedTextSize = 0;

    for (int i = 0; textToDecode[i] != '\0'; i++) {

        int bit = (textToDecode[i] - '0');
        if (bit == 0) {
            current = current->left;
        }
        else {
            current = current->right;
        }

        if (current->left == NULL && current->right == NULL) {
            decodedText[decodedTextSize] = current->value;
            decodedTextSize++;

            if (decodedTextSize >= decodedTextCapacity) {
                decodedTextCapacity *= 2;
                decodedText = (char*)realloc(decodedText, decodedTextCapacity);
            }
            current = root;
        }
    }

    decodedText[decodedTextSize] = '\0';
    return decodedText;
}

#endif
