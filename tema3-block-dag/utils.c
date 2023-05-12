#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./utils.h"

graph *createGraph(int nrNodes) {
    graph* newGraph = malloc(sizeof(graph));
    newGraph->nrNodes = nrNodes;
    newGraph->adl = malloc(sizeof(ATNode) * nrNodes);
    newGraph->futureAdl = malloc(sizeof(ATNode) * nrNodes);
    for (int i=0; i<nrNodes; i++) {
        newGraph->adl[i] = NULL;
        newGraph->futureAdl[i] = NULL;
    }
    return newGraph;
}

void insertAdjList(graph* DAG, int fromNodeIndex, int destNodeIndex) {
    TNode *newNode = malloc(sizeof(TNode));
    newNode->v = destNodeIndex;
    newNode->next = DAG->adl[fromNodeIndex];
    DAG->adl[fromNodeIndex] = newNode;
}

void insertFutureAdjList(graph* DAG, int destNodeIndex, int fromNodeIndex) {
    TNode *newNode = malloc(sizeof(TNode));
    newNode->v = fromNodeIndex;
    newNode->next = DAG->futureAdl[destNodeIndex];
    DAG->futureAdl[destNodeIndex] = newNode;
}

int getNodeIndex(char** dict, char* s, int nrNodes) {
    for (int i=0; i<nrNodes; i++) {
        if (strcmp(dict[i], s) == 0)
            return i;
    }
    return -1;
}

int DFS(graph *DAG, int idx, int* visited) {
    visited[idx] = 1;
    ATNode temp = DAG->adl[idx];
    while (temp != NULL) {
        if (visited[temp->v] == 1) {
            return 1;
        }
        if (visited[temp->v] == 0 && DFS(DAG, temp->v, visited)) {
            return 1;
        }
        temp = temp->next;
    }
    visited[idx] = 2;
    return 0;
}

void past(graph *DAG, int idx, int* visited, int* result) {
    visited[idx] = 1;
    ATNode temp = DAG->adl[idx];
    while (temp != NULL) {
        if (visited[temp->v] == 0) {
            past(DAG, temp->v, visited, result);
        }
        temp = temp->next;
    }
    result[idx]++;
}

void future(graph *DAG, int idx, int* visited, int* result) {
    visited[idx] = 1;
    ATNode temp = DAG->futureAdl[idx];
    while (temp != NULL) {
        if (visited[temp->v] == 0) {
            future(DAG, temp->v, visited, result);
        }
        temp = temp->next;
    }
    result[idx]++;
}

void anticone(int* pastVector, int* futureVector, int* anticoneVector, int nrNodes) {
    for (int i=0; i<nrNodes; i++) {
        if (pastVector[i] == 0) {
            if (futureVector[i] == 0)
                anticoneVector[i] = 1;
        }
    }
}

void tips(graph* DAG, int* tipsVector) {
    for (int i=0; i<DAG->nrNodes; i++) {
        if (DAG->futureAdl[i] == NULL)
            tipsVector[i] = 1;
    }
}

void lexBubbleSort(int* vector, int nrNodes, char** dict, int* auxVector) {
    int count = 0;
    for (int i=1; i<nrNodes; i++) {
        if (vector[i] > 0)
            count++;
    }

    zeros(auxVector, nrNodes);

    if (vector[0] > 0)
        auxVector[0] = 1;
    count = 1;
    for (int i=1; i<nrNodes; i++) {
        if (vector[i] > 0) {
            auxVector[count] = i;
            count++;
        }
    }

    // sortam de la index 1, genesis mereu va fi primul element.
    for (int i=1; i<nrNodes; i++) {
        for (int j=1; j<nrNodes-i-1; j++) {
            if (strcmp(getNodeName(dict, auxVector[j], nrNodes), getNodeName(dict, auxVector[j+1], nrNodes)) > 0) {
                int aux = auxVector[j];
                auxVector[j] = auxVector[j+1];
                auxVector[j+1] = aux;
            }
        }
    }
}

void zeros(int* vector, int nrNodes) {
    for (int i=0; i<nrNodes; i++) {
        vector[i] = 0;
    }
}

void printVectors(FILE* fout, char** dict, int nodeIndex, int* pastVector,
                  int* futureVector, int* anticoneVector, int* tipsVector, int nrNodes) {
    int* auxVector = malloc(sizeof(int) * nrNodes);

    // past
    lexBubbleSort(pastVector, nrNodes, dict, auxVector);
    fprintf(fout, "past(%s) :", getNodeName(dict, nodeIndex, nrNodes));
    if (auxVector[0] > 0) {
        fprintf(fout, " Genesis");
    }
    for (int i=1; i<nrNodes; i++) {
        if (auxVector[i] != nodeIndex && auxVector[i] > 0) {
            fprintf(fout, " %s", getNodeName(dict, auxVector[i], nrNodes));
        }
    }
    fprintf(fout, " \n");


    //future
    lexBubbleSort(futureVector, nrNodes, dict, auxVector);
    fprintf(fout, "future(%s) :", getNodeName(dict, nodeIndex, nrNodes));
    if (auxVector[0] > 0) {
        fprintf(fout, " Genesis");
    }
    for (int i=1; i<nrNodes; i++) {
        if (auxVector[i] != nodeIndex && auxVector[i] > 0) {
            fprintf(fout, " %s", getNodeName(dict, auxVector[i], nrNodes));
        }
    }
    fprintf(fout, " \n");

    //anticone
    lexBubbleSort(anticoneVector, nrNodes, dict, auxVector);
    fprintf(fout, "anticone(%s) :", getNodeName(dict, nodeIndex, nrNodes));
    if (auxVector[0] > 0) {
        fprintf(fout, " Genesis");
    }
    for (int i=1; i<nrNodes; i++) {
        if (auxVector[i] != nodeIndex && auxVector[i] > 0) {
            fprintf(fout, " %s", getNodeName(dict, auxVector[i], nrNodes));
        }
    }
    fprintf(fout, " \n");

    //tips
    fprintf(fout, "tips(G) :");
    for (int i=0; i<nrNodes; i++) {
        if (tipsVector[i] > 0) {
            fprintf(fout, " %s", getNodeName(dict, i, nrNodes));
        }
    }
    fprintf(fout, " \n");

    free(auxVector);
}

int card(int* vec, int nrNodes) {
    int k = 0;
    for (int i=0; i<nrNodes; i++)
        if (vec[i] > 0)
            k++;
    return k;
}

void calculateBlockScores(graph* DAG, int* pastVector, int* visited, int* scoresVector) {
    zeros(pastVector, DAG->nrNodes);
    zeros(visited, DAG->nrNodes);
    zeros(scoresVector, DAG->nrNodes);

    for (int i=0; i<DAG->nrNodes; i++) {
        zeros(pastVector, DAG->nrNodes);
        zeros(visited, DAG->nrNodes);

        past(DAG, i, visited, pastVector);
        int currentNodeScore = 0;
        for (int j=0; j<DAG->nrNodes; j++) {
            if (pastVector[j] > 0 && j != i) {
                currentNodeScore++;
            }
        }
        scoresVector[i] = currentNodeScore;
    }
}

int isBlueSet(int* blueSet, int idx, int nrNodes) {
    for (int i=0; i<nrNodes; i++) {
        if (blueSet[i] == idx) {
            return 1;
        }
    }
    return 0;
}

void blueSetIntersect(int* vector, int* blueSet, int vecSize, int* intersection) {
    zeros(intersection, vecSize);
    for (int i=0; i<vecSize; i++) {
        if (vector[i] > 0) {
            if (isBlueSet(blueSet, i, vecSize)) {
                intersection[i] = 1;
            }
        }
    }

}

void orderDAG(graph* DAG, int* blueSet, int k, char** dict, int* tipsVector) {
    int* pastVector = malloc(sizeof(int) * DAG->nrNodes);
    int* blockScores = malloc(sizeof(int) * DAG->nrNodes);
    int* visited = malloc(sizeof(int) * DAG->nrNodes);
    calculateBlockScores(DAG, pastVector, visited, blockScores);
    int maxScore = -1, maxScoreIdx = -1, blueSetIter = -1;

    for (int i=0; i<DAG->nrNodes; i++) {
        if (blockScores[i] > maxScore) {
            maxScore = blockScores[i];
            maxScoreIdx = i;
        }
    }
    blueSetIter++;
    blueSet[blueSetIter] = maxScoreIdx;

    int orderIdx = maxScoreIdx;
    while (orderIdx != 0) {
        ATNode temp = DAG->adl[orderIdx];
        maxScoreIdx = -1, maxScore = -1;
        while (temp != NULL) {
            if (blockScores[temp->v] > maxScore) {
                maxScore = blockScores[temp->v];
                maxScoreIdx = temp->v;
            }
            temp = temp->next;
        }
        blueSetIter++;
        blueSet[blueSetIter] = maxScoreIdx;

        orderIdx = maxScoreIdx;
    }

    int mainChainIter = blueSetIter;
    int* pastAuxVector = malloc(sizeof(int) * DAG->nrNodes);
    int* futureVector = malloc(sizeof(int) * DAG->nrNodes);
    int* visitedAux = malloc(sizeof(int) * DAG->nrNodes);
    int* anticoneVector = malloc(sizeof(int) * DAG->nrNodes);
    int* intersection = malloc(sizeof(int) * DAG->nrNodes);

    for (int i=mainChainIter-1; i>=0; i--) {
        zeros(pastVector, DAG->nrNodes);
        zeros(futureVector, DAG->nrNodes);
        zeros(anticoneVector, DAG->nrNodes);
        zeros(visited, DAG->nrNodes);

        past(DAG, blueSet[i], visited, pastVector);

        for (int j=1; j<DAG->nrNodes; j++) {
            if (pastVector[j] > 0 && !isBlueSet(blueSet, j, DAG->nrNodes)) {
                zeros(pastAuxVector, DAG->nrNodes);
                zeros(futureVector, DAG->nrNodes);
                zeros(anticoneVector, DAG->nrNodes);
                zeros(visitedAux, DAG->nrNodes);

                past(DAG, j, visitedAux, pastAuxVector);
                zeros(visitedAux, DAG->nrNodes);
                future(DAG, j, visitedAux, futureVector);
                anticone(pastAuxVector, futureVector, anticoneVector, DAG->nrNodes);


                zeros(intersection, DAG->nrNodes);
                blueSetIntersect(anticoneVector, blueSet, DAG->nrNodes, intersection);
                if (card(intersection, DAG->nrNodes) <= k) {
                    blueSetIter++;
                    blueSet[blueSetIter] = j;
                }
            }
        }
    }

    for (int i=0; i<DAG->nrNodes; i++) {
        if (i != maxScoreIdx && tipsVector[i] > 0) {
            if (!isBlueSet(blueSet, i, DAG->nrNodes)) {
                zeros(pastAuxVector, DAG->nrNodes);
                zeros(futureVector, DAG->nrNodes);
                zeros(anticoneVector, DAG->nrNodes);
                zeros(visitedAux, DAG->nrNodes);

                past(DAG, i, visitedAux, pastAuxVector);
                zeros(visitedAux, DAG->nrNodes);
                future(DAG, i, visitedAux, futureVector);
                anticone(pastAuxVector, futureVector, anticoneVector, DAG->nrNodes);

                zeros(intersection, DAG->nrNodes);
                blueSetIntersect(anticoneVector, blueSet, DAG->nrNodes, intersection);
                if (card(intersection, DAG->nrNodes) <= k) {
                    blueSetIter++;
                    blueSet[blueSetIter] = i;
                }
            }
        }
    }

    for (int i=0; i<DAG->nrNodes; i++) {
        for (int j=0; j<DAG->nrNodes-i-1; j++) {
            if (strcmp(getNodeName(dict, blueSet[j], DAG->nrNodes),
                       getNodeName(dict, blueSet[j+1], DAG->nrNodes)) > 0) {
                int aux = blueSet[j];
                blueSet[j] = blueSet[j+1];
                blueSet[j+1] = aux;
            }
        }
    }

    free(pastVector);
    free(pastAuxVector);
    free(futureVector);
    free(visited);
    free(visitedAux);
    free(anticoneVector);
    free(blockScores);
    free(intersection);
}

char* getNodeName(char** dict, int index, int nrNodes) {
    return dict[index];
}

void freeGraph(graph* DAG) {
    for (int i=0; i<DAG->nrNodes; i++) {
        ATNode temp = DAG->adl[i];
        while (temp != NULL) {
            ATNode remove = temp;
            temp = temp->next;
            free(remove);
        }
        DAG->adl[i] = NULL;

        temp = DAG->futureAdl[i];
        while (temp != NULL) {
            ATNode remove = temp;
            temp = temp->next;
            free(remove);
        }
        DAG->futureAdl[i] = NULL;
    }

    free(DAG->adl);
    free(DAG->futureAdl);
    free(DAG);
}

// functie debug
void printAdjList(graph *G)
{
    for (int i=0; i<G->nrNodes; i++) {
        ATNode temp = G->adl[i];
        printf("%d -> ", i);
        while (temp) {
            printf("%d ", temp->v);
            temp = temp->next;
        }
        printf("\n");
    }
}

// functie debug
void printFutureAdjList(graph *G)
{
    for (int i=0; i<G->nrNodes; i++) {
        ATNode temp = G->futureAdl[i];
        printf("%d <- ", i);
        while (temp) {
            printf("%d ", temp->v);
            temp = temp->next;
        }
        printf("\n");
    }
}