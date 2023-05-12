#ifndef SORTED_LIST_H
#define SORTED_LIST_H

#include <stdlib.h>
#include <assert.h>

/*
    IMPORTANT!

    As we stick to pure C, we cannot use templates. We will just asume
    some type T was previously defined.
*/

// -----------------------------------------------------------------------------

typedef struct node {
    T value;
    struct node* next;
} TNode, *TSortedList;


TSortedList create(T value) {
    TSortedList newList = NULL;
    newList = (TNode*) malloc(sizeof(TNode));

    newList->next = NULL;
    newList->value = value;
    return newList;
}

int isEmpty(TSortedList list) {
    if (list == NULL) return 1;
    return 0;
}

int contains(TSortedList list, T value) {
    if (list == NULL) return 0;

    TNode *temp = list;
    while(temp != NULL) {
        if (temp->value == value)
            return 1;
        temp = temp->next;
    }
    return 0;
}

TSortedList insert(TSortedList list, T value) {
    TNode *temp = malloc(sizeof(TNode));
    temp->value = value;
    temp->next = NULL;

    if (list == NULL) {
        list = temp;
    }
    else {
        TNode *prev = NULL;
        TNode *current = list;

        while(current && current->value <= value) {
            prev = current;
            current = current->next;
        }

        if (!current)
            prev->next = temp;  //final
        else {
            if (prev) {
                temp->next = prev->next;
                prev->next = temp;
            }
            else {
                temp->next = list;  //start
                list = temp;
            }
        }
    }
    return list;
}

TSortedList deleteOnce(TSortedList list, T value) {
    TNode *temp = list;
    if (temp == NULL) return NULL;

    if (temp->value == value) {
        TNode *remove = list;
        list = list->next;
        free(remove);
        return list;
    }
    else {
        while(temp->next != NULL){
            if (temp->next->value == value) {
                TNode *remove = temp->next;
                temp->next = temp->next->next;
                free(remove);
                break;
            }
            temp = temp->next;
        }
        return list;
    }
}

long length(TSortedList list) {
    long len = 0;
    TNode *temp = list;
    if (temp == NULL) return 0;
    else {
        while(temp->next != NULL) {
            len++;
            temp = temp->next;
        }
        len++;
        return len;
    }
}

T getNth(TSortedList list, unsigned int N) {
    TNode *temp = list;
    T elem = temp->value;

    for(int i=1; i<N; i++) {
        temp = temp->next;
        elem = temp->value;
    }
    return elem;
}

TSortedList freeTSortedList(TSortedList list) {
    TNode *temp = NULL;
    while(list != NULL) {
        temp = list;
        list = list->next;
        free(temp);
    }
    list = NULL;
    return list;
}

#endif
