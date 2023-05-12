#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>
#include <stdlib.h>



typedef struct StackNode{
	Item elem;
	struct StackNode *next;
}StackNode;

typedef StackNode TCelSt; // Redenumirea unui nod la notatiile din curs 
typedef StackNode *ACelSt; // Redenumirea unui pointer la nod la notatiile din curs

typedef struct Stack{
	StackNode* head;  // Varful stivei
	long size; // Numarul de elemente din stiva
}Stack;

typedef Stack TStiva; // Redenumirea stivei la notatiile din curs
typedef Stack *ASt;//  // Redenumirea unui pointer la stiva la notatiile din curs

Stack* createStack(void){

	// Just allocate memory and return the pointer to the new stack.
	Stack *newStack = malloc(sizeof(struct Stack));
	newStack->head = NULL;
	newStack->size = 0;
	return newStack;
}

int isStackEmpty(Stack* stack){

	// Check if the head is NULL.
	if (stack->head == NULL) {
        return 1;  // stack is empty.
    } else {
        return 0;  // stack is not empty.
    }
}

void push(Stack *stack, Item elem){
	// Allocate memory for a new node.
    StackNode *newNode = malloc(sizeof(struct StackNode));
    // Set the element field.
	newNode->elem = elem;
	// Set the next node as being the current first node.
    newNode->next = stack->head;
	// Set the first node as the new node.
    stack->head = newNode;
	// Increment the size of the stack.
    stack->size++;
}

Item top(Stack *stack){

	// Check if the stack is empty.
    if (isStackEmpty(stack)) {

		// If empty, output error message and exit.
        printf("Stack is empty.\n");
        exit(EXIT_FAILURE);
    }

	// Stack is not empty, return the first element.
    return stack->head->elem;
}

void pop(Stack *stack){

	// Check if the stack is empty.
    if (isStackEmpty(stack)) {

		// If empty, output error message and exit.
        printf("Stack is empty.\n");
        exit(EXIT_FAILURE);
    }

	// Extract the head of the stack.
    StackNode *node = stack->head;
	// The new head pointer now points to the second element in the stack.
    stack->head = stack->head->next;

	// Free the memory of the extracted node.
    free(node);
	// Decrement the stack's size.
    stack->size--;
}

void destroyStack(Stack *stack){

	// While the stack still contains elements.
	while (!isStackEmpty(stack)) {

		// The pop operation will take care to free the node's memory.
        pop(stack);
    }

	// Free the memory of the stack structure.
    free(stack);
}

#endif 
