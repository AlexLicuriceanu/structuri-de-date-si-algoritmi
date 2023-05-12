#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Util.h"
#include "Graph.h"

TGraphL* createGraphAdjList(int numberOfNodes) {
    // Allocate memory for the graph
    TGraphL* graph = (TGraphL*)malloc(sizeof(TGraphL));

    // Initialize the number of nodes in the graph
    graph->nn = numberOfNodes;

    // Allocate memory for the adjacency list
    graph->adl = (ATNode*)malloc(numberOfNodes * sizeof(ATNode));

    // Initialize the adjacency list to NULL
    for (int i = 0; i < numberOfNodes; i++) {
        graph->adl[i] = NULL;
    }

    // Return the created graph
    return graph;
}

void addEdgeList(TGraphL* graph, int v1, int v2) {
	TNode* n1 = (TNode*)malloc(sizeof(TNode));
	TNode* n2 = (TNode*)malloc(sizeof(TNode));

	n1->v = v2; // Set destination
	n1->next = graph->adl[v1]; // Insert it in the list
	graph->adl[v1] = n1; // Update list pointer

	n2->v = v1;
	n2->next = graph->adl[v2];
	graph->adl[v2] = n2;
}

void dfsRecHelper(TGraphL* graph, int* visited, List* path, int s) {

	visited[s] = 1;
	enqueue(path,s);

	for(ATNode it = graph->adl[s]; it != NULL; it = it->next) {
		if(visited[it->v] != 1) {
			dfsRecHelper(graph,visited,path, it->v);
		}
	}
}

List *dfsRecursive(TGraphL* graph, int s) {

	List* path = createList();
	int *visited = (int*) malloc(graph->nn*sizeof(int));
	
	for(int i = 0; i < (graph->nn); i++) {
		visited[i] = 0;
	}

	dfsRecHelper(graph,visited,path, s);
	free(visited);
	return path;
}

List *bfs(TGraphL* graph, int s){

	List *path = createList();
	int *visited = (int*)malloc(graph->nn*sizeof(int));

	for(int i = 0; i < graph->nn; i++)
		visited[i] = 0;

	Queue *q = createQueue();

	visited[s] = 1;
	enqueue(q,s);

	while(!isQueueEmpty(q)){
		s = front(q);
		dequeue(q);
		enqueue(path,s);

		for(ATNode it = graph->adl[s]; it != NULL; it = it->next) {
			if(visited[it->v] != 1) {
				visited[it->v] = 1;
				enqueue(q,it->v);
			}
		}
	}

	free(visited);
	destroyQueue(q);
	return path;
}



void destroyGraphAdjList(TGraphL* graph){
    // Free memory for each node in the adjacency list
    for (int i = 0; i < graph->nn; i++) {
        TNode* currentNode = graph->adl[i];
		
        while (currentNode != NULL) {
            TNode* nextNode = currentNode->next;
            free(currentNode);
            currentNode = nextNode;
        }
    }

    // Free memory for the adjacency list itself
    free(graph->adl);

    // Free memory for the graph
    free(graph);
}

void removeEdgeList(TGraphL* graph, int v1, int v2){
	TNode* it = graph->adl[v1];
	TNode* prev = NULL;
	while(it != NULL && it->v != v2){
		prev = it;
		it = it->next;
	}

	if(it == NULL)return;

	if(prev != NULL)
		prev->next = it->next;
	else
		graph->adl[v1] = it->next;
	free(it);

	it = graph->adl[v2];
	prev = NULL;
	while(it != NULL && it->v != v1){
		prev = it;
		it = it->next;
	}
	if(it == NULL) return;
	if(prev != NULL)
		prev->next = it->next;
	else
		graph->adl[v2] = it->next;

	free(it);
}

void removeNodeList(TGraphL* graph, int v){
	for(int i = 0; i < graph->nn;i++){
		removeEdgeList(graph,v,i);
	}
}
