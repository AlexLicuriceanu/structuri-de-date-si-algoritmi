#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct node {
    long value;
    struct node* next;
} Node, *List;

List createHugeList(long size) {
    long idx;
    List list, aux;

    list = (Node*) malloc(sizeof(Node));
    list->value = 0l;

    aux = list;
    for (idx = 1l; idx < size; idx++, aux=aux->next) {
        aux->next = (Node*) malloc(sizeof(Node));
        aux->next->value = (idx % 5l) - 2l;
    }
    aux->next = NULL;
    return list;
}

void deleteHugeList(List list) {
    List aux = list;
    while(aux) {
        list = aux->next;
        free(aux);
        aux = list;
    } 
}

long lengthIterative(List list) {
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

long lengthStackRecursive(List list) {
    // TODO : Cerința 3
    return 0l;
}

long lengthTailRecursive(List list) {
    // TODO : Cerința 3
    return 0l;
}

void evalLengthFunction(long SIZE, char name[], long(*length)(List)) {
    clock_t t0, t1;
    List list = createHugeList(SIZE);
    long size;
    t0 = clock();
    size = (*length)(list);
    t1 = clock();
    deleteHugeList(list);
    printf("Size: %ld | %20s: %lf\n", size, name, (double)(t1-t0)/CLOCKS_PER_SEC);
}

int main(int argc, char* argv[]) {
    static const long SIZE = 200000l;                              // size of list

    evalLengthFunction(SIZE, "Iterative", &lengthIterative);
    evalLengthFunction(SIZE, "Tail Recursive", &lengthTailRecursive);
    evalLengthFunction(SIZE, "Stack Recursive", &lengthStackRecursive);

    return 0;
}
