#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/* Struct for undirected graph */
typedef struct {
    char** map; /* the map of the graph '.'->empty '*'->wall */
    int** distances /* the distance of every empty cell of the map */;
} Graph;

/* Reads the map from file and returns it as is */
char** generateMapFromFile(FILE *inputFile, int n, int m) {
    char** map = (char**)malloc(sizeof(char) * n * m);
    return NULL;
}

int main(int argc, char** argv) {
    FILE* inputFile;
    int n;
    int m;
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
    if (2 != fscanf(inputFile, "%d %d\n", &n, &m)) {
        return EXIT_FAILURE;
    }
    assert(n > 0 && m > 0);
    graph.map = generateMapFromFile(inputFile, n, m);
    processMap(graph.map);

    return EXIT_SUCCESS;
}