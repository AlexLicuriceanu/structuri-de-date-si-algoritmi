#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./utils.h"

int main(int argc, char *argv[]) {
    FILE *fin, *fout;
    fin = fopen("blockdag.in", "r");
    fout = fopen("blockdag.out", "w");

    int nrNodes = 0;
    fscanf(fin, "%d\n", &nrNodes);

    char temp[512];
    fgets(temp, 512, fin);
    temp[strlen(temp)-1] = '\0';

    char** dict = malloc(sizeof(char*) * nrNodes);
    char* p = strtok(temp, " ");
    int k = 0;
    while (p != NULL) {
        dict[k] = malloc(sizeof(char) * (strlen(p) + 1));
        strcpy(dict[k++], p);
        p = strtok(NULL, " ");
    }

    graph *DAG = createGraph(nrNodes);

    for (int i=0; i<nrNodes; i++) {
        fgets(temp, 512, fin);
        temp[strlen(temp)-1] = '\0';

        p = strtok(temp, " ");
        int fromNodeIndex = getNodeIndex(dict, p, nrNodes);
        p = strtok(NULL, " ");



        if (fromNodeIndex != 0) {
            while (p != NULL) {
                if (strcmp(p, ":") != 0) {

                    int destNodeIndex = getNodeIndex(dict, p, nrNodes);
                    insertAdjList(DAG, fromNodeIndex, destNodeIndex);
                    insertFutureAdjList(DAG, destNodeIndex, fromNodeIndex);
                }
                p = strtok(NULL, " ");
            }
        }
    }

    if (strcmp(argv[1], "-c1") == 0) {
        int* visited = malloc(sizeof(int) * nrNodes);
        zeros(visited, nrNodes);

        int cyclic = 0;
        for (int i=0; i<nrNodes; i++) {
            if (!visited[i]) {
                if (DFS(DAG, i, visited) == 1) {
                    cyclic = 1;
                    break;
                }
            }
        }

        if (!cyclic == 1)
            fprintf(fout, "correct\n");
        else
            fprintf(fout, "impossible\n");

        free(visited);
    }
    else if (strcmp(argv[1], "-c2") == 0) {
        char nodeName[11];
        strcpy(nodeName, argv[2]);

        int startNodeIndex = getNodeIndex(dict, nodeName, nrNodes);
        int* pastVector = malloc(sizeof(int) * nrNodes);
        int* futureVector = malloc(sizeof(int) * nrNodes);
        int* anticoneVector = malloc(sizeof(int) * nrNodes);
        int* tipsVector = malloc(sizeof(int) * nrNodes);
        int* visited = malloc(sizeof(int) * nrNodes);

        // initializari cu 0
        zeros(pastVector, nrNodes);
        zeros(futureVector, nrNodes);
        zeros(anticoneVector, nrNodes);
        zeros(tipsVector, nrNodes);
        zeros(visited, nrNodes);

        // rularea functiilor
        past(DAG, startNodeIndex, visited, pastVector);

        // reset vector de vizitati pentru future()
        zeros(visited, nrNodes);

        future(DAG, startNodeIndex, visited, futureVector);
        anticone(pastVector, futureVector, anticoneVector, nrNodes);
        tips(DAG, tipsVector);

        // se apeleaza func. de scriere in fisier
        printVectors(fout, dict, startNodeIndex, pastVector,
                     futureVector, anticoneVector, tipsVector, nrNodes);

        // eliberam memoria
        free(pastVector);
        free(futureVector);
        free(anticoneVector);
        free(tipsVector);
        free(visited);
    }
    else if (strcmp(argv[1], "-c3") == 0) {
        int nrK = atoi(argv[2]);
        int* blueSet = malloc(sizeof(int) * nrNodes);
        int* tipsVector = malloc(sizeof(int) * nrNodes);
        zeros(blueSet, nrNodes);
        zeros(tipsVector, nrNodes);

        tips(DAG, tipsVector);
        orderDAG(DAG, blueSet, nrK, dict, tipsVector);

        fprintf(fout, "Genesis");
        for (int i=0; i<DAG->nrNodes; i++) {
            if (blueSet[i] > 0)
                fprintf(fout, " %s", getNodeName(dict, blueSet[i], DAG->nrNodes));
        }
        fprintf(fout, " \n");

        free(blueSet);
        free(tipsVector);
    }


    for (int i=0; i<nrNodes; i++) {
        free(dict[i]);
    }
    free(dict);
    freeGraph(DAG);

    fclose(fin);
    fclose(fout);
    return 0;
}
