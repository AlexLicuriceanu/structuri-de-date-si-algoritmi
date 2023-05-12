#ifndef INC_3TEMA3SDA_UTILS_H
#define INC_3TEMA3SDA_UTILS_H

typedef struct node{
    int v;
    struct node* next;
} TNode, *ATNode;

typedef struct {
    int nrNodes;
    ATNode *adl;
    ATNode *futureAdl;
} graph;

graph *createGraph();
int getNodeIndex();
void insertAdjList();
void insertFutureAdjList();
void printAdjList();
void printFutureAdjList();
int DFS();
void freeGraph();
void past();
void future();
void lexBubbleSort();
void anticone();
void tips();
int card();
void calculateBlockScores();
int isBlueSet();
void blueSetIntersect();
void orderDAG();
void printVectors();
char* getNodeName();
void zeros();

#endif //INC_3TEMA3SDA_UTILS_H
