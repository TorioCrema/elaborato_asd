#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#define EMPTY '.'
#define WALL '*'
#define EXPANDED_WALL 'x'

/* Struct for undirected graph */
typedef struct {
    char* map; /* the map of the graph '.'->empty '*'->wall */
    int* distances /* the distance of every cell of the map from the source */;
    int n;
    int m;
} Graph;

/* Returns the index to use on the graph's map or distances arrays */
int getIndex(int x, int y, Graph* graph) {
    assert((x >= 0) && (x < graph->n));
    assert((y >= 0) && (y < graph->m));
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
            }
        }
        return 1;
    }
}

/* Frees the memory allocated for the graph. */
void deleteGraph(Graph* graph) {
    free(graph->map);
    free(graph->distances);
}

/* Reads inputFile and saves the characters in the graph's map. */
void generateMapFromFile(FILE* inputFile, Graph* graph) {
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

/* Returns true if the cell at the given coords is empty, false otherwise. */
bool isEmpty(int x, int y, Graph* graph) {
    int i;
    int o;
    for (i = x - 1; i <= x + 1; i++) {
        for (o = y - 1; o <= y + 1; o++) {
            if (graph->map[getIndex(i, o, graph)] == WALL) {
                return false;
            }
        }
    }
    return true;

}

/* Fills row at index x with walls */
void fillRow(int x, Graph* graph) {
    int o;
    for (o = 0; o < graph->m; o++) {
        if (graph->map[getIndex(x, o, graph)] == EMPTY) {
            graph->map[getIndex(x, o, graph)] = EXPANDED_WALL;
        }
    }
}

/* Fills column at index y with walls */
void fillColumn(int y, Graph* graph) {
    int i;
    for (i = 0; i < graph->n - 1; i++) {
        if (graph->map[getIndex(i, y, graph)] == EMPTY) {
            graph->map[getIndex(i, y, graph)] = EXPANDED_WALL;
        }
    }
}

/* 'Expands' the walls of the map so that it can be explored as a 3x3 entity. */
void processMap(Graph* graph) {
    int i;
    int o;
    for (i = 1; i < graph->n - 1; i++) {
        for (o = 1; o < graph->m - 1; o++) {
            if (graph->map[getIndex(i, o, graph)] == EMPTY
                    && !isEmpty(i, o, graph)) {
                graph->map[(getIndex(i, o, graph))] = EXPANDED_WALL;
            }
        }
    }
    /* Fill edges of the map */
    fillRow(0, graph);
    fillRow(graph->n - 1, graph);
    fillColumn(0, graph);
    fillColumn(graph->m - 1, graph);
}

/* Prints the graph's map */
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
    if (!initGraph(&graph))
        return EXIT_FAILURE;
    fflush(stdout);
    generateMapFromFile(inputFile, &graph);
    fclose(inputFile);
    printMap(&graph);
    processMap(&graph);
    printf("\n");
    printMap(&graph);

    /* Remember to free malloc-ed memory! */
    deleteGraph(&graph);
    return EXIT_SUCCESS;
}