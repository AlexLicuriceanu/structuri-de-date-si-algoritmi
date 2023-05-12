#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdlib.h>
#include <math.h>

/* We assume there is a defined primitive type Item. */
typedef struct {
	int prior; // Item priority
	Item data; // Item d
}ItemType;

typedef struct heap{
	long maxHeapSize; // capacity
	long size; // number of elements
	ItemType *elem; // array of elements
} PriQueue, *APriQueue;


PriQueue* makeQueue(int maxHeapSize){

	PriQueue *pq = malloc(sizeof(PriQueue));

	pq->maxHeapSize = maxHeapSize;
	pq->size = 0;
	pq->elem = malloc(maxHeapSize * sizeof(ItemType));
	
	return pq;
}

int getLeftChild(int i) {
    return 2*i + 1;
}

int getRightChild(int i) {
    return 2*i + 2;
}

int getParent(int i) {
    return (i-1)/2;
}

ItemType getMax(APriQueue h) {
	return h->elem[0];
}

void siftUp(APriQueue h, int idx) {
    if (idx <= 0) {
        return; // The root node has no parent
    }

    int parent = getParent(idx);

    if (h->elem[parent].prior < h->elem[idx].prior) {

        // Swap the current node with its parent
        ItemType tmp = h->elem[parent];
        h->elem[parent] = h->elem[idx];
        h->elem[idx] = tmp;

        // Recursively sift up the swapped node
        siftUp(h, parent);
    }
}


void insert(PriQueue *h, ItemType x) {
	if (h->size == h->maxHeapSize) {
		h->maxHeapSize <<= 1;
		h->elem = (ItemType *) realloc(h->elem, h->maxHeapSize * sizeof(ItemType));
	}

	h->elem[h->size] = x;
	siftUp(h, h->size);
	h->size++;
}

void siftDown(APriQueue h, int idx){
	int left = getLeftChild(idx);
	int right = getRightChild(idx);
	int largest = idx;

	if (left <= h->size && h->elem[left].prior > h->elem[largest].prior) {
		largest = left;
	}

	if (right <= h->size && h->elem[right].prior > h->elem[largest].prior) {
		largest = right;
	}

	if (largest != idx) {
		// swap the current node with the largest child
		ItemType temp = h->elem[idx];
		h->elem[idx] = h->elem[largest];
		h->elem[largest] = temp;

		// recursively sift down the swapped node
		siftDown(h, largest);
	}
}

void removeMax(APriQueue h) {
    if (h->size == 0) {
        return; // heap is empty
    }

    h->elem[0] = h->elem[h->size - 1]; // replace root with last element
    h->size--; // reduce heap size

    siftDown(h, 0); // restore heap property
}

void freeQueue(APriQueue h){
    free(h->elem);
    free(h);
}

#endif

