#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct QueueNode{
	Item elem;
	struct QueueNode *next;
}QueueNode;

typedef QueueNode TcelQ;
typedef QueueNode *ACelQ;

typedef struct Queue{
	QueueNode *front;
	QueueNode *rear;
	long size;
}Queue;

typedef Queue TCoada;
typedef Queue *AQ;


Queue* createQueue(void){
	// Allocate memory for a queue structure.
	Queue *newQueue = malloc(sizeof(struct Queue));
	// Set the front, rear and size fields.
	newQueue->rear = NULL;
	newQueue->front = NULL;
	newQueue->size = 0;
	return newQueue;
} 

int isQueueEmpty(Queue *q){
	// Check if both front and read point to NULL.
	if (q->front == NULL && q->rear == NULL) {

		// Queue is empty, return 1.
		return 1;
	}

	// Queue has elements, return 0.
	return 0;
}

void enqueue(Queue *q, Item elem){
	
	// Allocate memory for a new node.
	QueueNode *newNode = malloc(sizeof(struct QueueNode));

	// Set the new node's elem and next fields.
    newNode->elem = elem;
    newNode->next = NULL;

	// If the queue is empty, set both front and rear pointers to the new node.
    if (isQueueEmpty(q)) {
        q->front = newNode;
        q->rear = newNode;
    } else {

		// Else, place the node at the end of the queue.
        q->rear->next = newNode;
        q->rear = newNode;
    }

	// Increment the queue's size.
    q->size++;
}

Item front(Queue *q){

	// Check if the queue is empty.
	if(isQueueEmpty(q)){
		// Output an error and exit.
		printf("Queue is empty.\n");
		exit(EXIT_FAILURE);
	}

	// If the queue is not empty, return the pointer to the first element.
	return q->front->elem;
}

void dequeue(Queue *q){

	// Check if the queue is empty.
	if(isQueueEmpty(q)){
		// Output an error and exit.
		printf("Queue is empty.\n");
	}

	// Get the pointer to the first node.
	QueueNode *temp = q->front;
	// Update the head of the queue to the second element.
	q->front = q->front->next;

	// Free the extracted node's memory.
	free(temp);
	// Decrement the queue's size.
	q->size--;

	// Check if the queue is empty after extracting and update the rear pointer.
	if(q->front == NULL){
		q->rear = NULL;
	}
}

void destroyQueue(Queue *q){
	// Sequentially dequeue all remaining nodes. 
    while(!isQueueEmpty(q)){

		// The dequeue operation will take care of freeing the memory of the node.
        dequeue(q);
    }

	// Free the queue structure's allocated memory.
    free(q);
}

#endif
