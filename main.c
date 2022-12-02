#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/* Struct for undirected graph */
typedef struct {
    char* map; /* the map of the graph '.'->empty '*'->wall */
    int* distances /* the distance of every cell of the map from the source */;
    int n;
    int m;
} Graph;

int getIndex(int x, int y, Graph* graph) {
    return x * graph->m + y;
}

/*
    Allocates the memory needed to make use of the graph
    and initializes it.
*/
int initGraph(Graph* graph) {
    int i;
    int o;
    graph->map = (char*)malloc(graph->n * graph->m * sizeof(char));
    graph->distances = (int*)malloc(graph->m * graph->n * sizeof(int));
    if (graph->map == NULL || graph->distances == NULL) {
        return 0;
    } else {
        for (i = 0; i < graph->n; i++) {
            for (o = 0; o < graph->m; o++) {
                graph->distances[getIndex(i, o, graph)] = -1;
                fflush(stdout);
            }
        }
        return 1;
    }
}

/*
    Frees the memory allocated for the graph.
*/
void deleteGraph(Graph* graph) {
    free(graph->map);
    free(graph->distances);
}

/*
    Reads inputFile and saves the characters in the graph's map.
*/
void generateMapFromFile(FILE *inputFile, Graph* graph) {
    int i;
    int o;
    char tmp;
    for (i = 0; i < graph->n; i++) {
        for (o = 0; o < graph->m; o++) {
            /* read char and save into map at it's coords */
            fscanf(inputFile, "%c", &graph->map[getIndex(i, o, graph)]);
        }
        /* read end of line */
        fscanf(inputFile, "%c", &tmp);
    }
}

/*
    Prints the graph's map
*/
void printMap(Graph* graph) {
    int i;
    int o;
    for (i = 0; i < graph->n; i++) {
        for (o = 0; o < graph->m; o++) {
            printf("%c", graph->map[getIndex(i, o, graph)]);
        }
        printf("\n");
    }
}

int main(int argc, char** argv) {
    FILE* inputFile;
    Graph graph;
    if (argc == 2) {
        inputFile = fopen(argv[1], "r");
        if (inputFile == NULL) {
            return EXIT_FAILURE;
        }
    } else {
        return EXIT_FAILURE;
    }
    /* Read n and m from file */
    if (2 != fscanf(inputFile, "%d%d ", &graph.n, &graph.m)) {
        return EXIT_FAILURE;
    }
    printf("%d %d\n", graph.n, graph.m);
    assert(graph.n > 0);
    assert(graph.m > 0);
    initGraph(&graph);
    fflush(stdout);
    generateMapFromFile(inputFile, &graph);
    fclose(inputFile);
    printMap(&graph);
    /*processMap(graph.map);*/

    /* Remember to free malloc-ed memory! */
    deleteGraph(&graph);
    return EXIT_SUCCESS;
}