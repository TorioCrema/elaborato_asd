#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#define EMPTY '.'
#define WALL '*'
#define EXPANDED_WALL 'x'

typedef enum { NORTH, EAST, SOUTH, WEST } Direction;
typedef struct {
    int capacity;
    int head, tail;
    int *data;
} Queue;

/* Creates an epty queue. */
Queue *queue_create( void )
{
    Queue *q = (Queue*)malloc(sizeof(*q));
    assert(q != NULL);

    q->capacity = 2;
    q->data = (int*)malloc(q->capacity * sizeof(*(q->data)));
    assert(q->data != NULL);
    q->head = q->tail = 0;
    return q;
}

/* Frees the space previously allocated for the queue. */
void queue_destroy( Queue *q )
{
    assert(q != NULL);

    free(q->data);
    q->data = NULL;
    q->head = q->tail = -1;
    q->capacity = 0;
    free(q);
}

/* Returns the number of elemets in the queue. */
int queue_size( const Queue *q )
{
    assert(q != NULL);

    if (q->head <= q->tail) {
        return (q->tail - q->head);
    } else {
        return (q->capacity - (q->head - q->tail));
    }
}

/* Returns true (nonzero) if and only if the queue is empty. */
static int queue_is_full( const Queue *q )
{
    assert(q != NULL);

    /* return q->head == ((q->tail + 1) % q->capacity); */
    return (queue_size(q) == (q->capacity - 1));
}

int queue_is_empty( const Queue *q )
{
    assert(q != NULL);

    return q->tail == q->head;
}

/*  Resizes the buffer of the queue q to the new dimension new_capaticy.
    All the data is copied for the old buffer to the new one. */
static void queue_resize( Queue *q, int new_capacity )
{
    int cur_size, cur_capacity;
    int *new_data;

    assert(q != NULL);
    cur_size = queue_size(q);
    assert( new_capacity >= cur_size );
    cur_capacity = q->capacity;
    new_data = (int*)malloc(new_capacity * sizeof(int));
    assert(new_data != NULL);
    /* Copy the old data from the old buffer to the new one */
    if (q->head <= q->tail) {
        /* If the all the data is within a continuos segment of
           the array, we can copy once */
        memcpy(new_data,
               q->data + q->head,
               cur_size * sizeof(int));
    } else {
        /* If the data is saved in two segments, we must make use
           of two different copy operations */
        memcpy(new_data,
               q->data + q->head,
               (cur_capacity - q->head) * sizeof(int));
        memcpy(new_data + (cur_capacity - q->head),
               q->data,
               q->tail * sizeof(int));
    }
    free(q->data);
    q->capacity = new_capacity;
    q->data = new_data;
    q->head = 0;
    q->tail = cur_size;
}

/* Add the value val to the queue q */
void queue_enqueue( Queue *q, int val )
{
    assert(q != NULL);

    if (queue_is_full(q)) {
        queue_resize(q, q->capacity * 2);
    }

    q->data[q->tail] = val;
    q->tail = (q->tail + 1) % q->capacity;
}

/* Removes and returns the first element of the queue */
int queue_dequeue( Queue *q )
{
    int result;

    assert(q != NULL);
    assert( ! queue_is_empty(q) );

    result = q->data[q->head];
    q->head = (q->head + 1) % q->capacity;

    if (queue_size(q) <= q->capacity / 4) {
        queue_resize(q, q->capacity / 2);
    }
    return result;
}

/* Prints the contents of the queue q. */
void queue_print(const Queue *q)
{
    int i;

    assert(q != NULL);

    printf("HEAD << ");
    for (i=q->head; i != q->tail; i = (i+1) % q->capacity) {
        printf("%d ", q->data[i]);
    }
    printf("<< TAIL\n");
}

/* Struct for undirected graph */
typedef struct {
    char* map; /* the map of the graph '.'->empty '*'->wall */
    int* dist /* the distance of every cell of the map from the source */;
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
    graph->dist = (int*)malloc(graph->m * graph->n * sizeof(int));
    if (graph->map == NULL || graph->dist == NULL) {
        return 0;
    } else {
        for (i = 0; i < graph->n; i++) {
            for (o = 0; o < graph->m; o++) {
                graph->dist[getIndex(i, o, graph)] = -1;
            }
        }
        return 1;
    }
}

/* Frees the memory allocated for the graph. */
void deleteGraph(Graph* graph) {
    free(graph->map);
    free(graph->dist);
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

void printDist(Graph* graph) {
    int i;
    int o;
    for (i = 0; i < graph->n; i++) {
        for (o = 0; o < graph->m; o++) {
            printf("%d ", graph->dist[getIndex(i, o, graph)]);
        }
        printf("\n");
    }
}

/* Returns the index of the neighbouring cell of x in the given Direction.
Returns -1 if the direction given is not valid. */
int getNeighbour(const int x, const Direction direction, Graph* graph) {
    int neighbour;
    switch (direction) {
    case NORTH:
        neighbour = x - graph->m;
        /* Can't go north from the first row of the map. */
        assert(neighbour >= 0);
        break;
    case EAST:
        /* Can't go east from the last column of the map. */
        assert(x % graph->m != graph->m - 1);
        neighbour = x + 1;
        assert(neighbour < graph->n * graph->m);
        break;
    case SOUTH:
        neighbour = x + graph->m;
        /* Can't go south from the last row of the map. */
        assert(neighbour < graph->n * graph->m);
        break;
    case WEST:
        /* Can't go west from the first column of the map. */
        assert(x % graph->m != 0);
        neighbour = x - 1;
        assert(neighbour >= 0);
        break;
    default:
        /* Called function with unexpected direction. */
        neighbour = -1;
        break;
    }
    return neighbour;
}

/* Returns true if the cell at the given index was visited, false otherwise. */
bool wasVisited(const int index, Graph* graph) {
    if (graph->dist[index] >= 0)
        return true;
    return false;
}

/*  Explores the graph through the BFS algorithm, starting from
    the source s. Returns true if the destination d was reached. */
bool bfs(Graph* graph, int s, int d) {
    Queue* q;
    int currCell;
    int currNeighbour;
    int i;
    int test = 0;
    printf("Starting bfs with s: %d, d:%d\n", s, d);
    fflush(stdout);
    q = queue_create();
    graph->dist[s] = 0;
    queue_enqueue(q, s);
    while (!queue_is_empty(q)) {
        test++;
        currCell = queue_dequeue(q);
        /* For every cell next to the current one (for every Direction). */
        for (i = 0; i < 4; i++) {
            currNeighbour = getNeighbour(currCell, i, graph);
            if (!wasVisited(currNeighbour, graph)) {
                /* If the current neighbour wasn't visited already. */
                if (graph->map[currNeighbour] == EMPTY) {
                    graph->dist[currNeighbour] = graph->dist[currCell] + 1;
                    queue_enqueue(q, currNeighbour);
                }
            }
        }
    }
    queue_destroy(q);
    if (wasVisited(d, graph)) {
        return true;
    }
    return false;   
}

void printPath(int d, Graph* graph) {
    Direction stepDir;
    char dirToPrint;
    if (graph->dist[d] == 0) {
        return;
    }
    for (stepDir = 0; stepDir < 4; stepDir++) {
        if (graph->dist[getNeighbour(d, stepDir, graph)] == graph->dist[d] - 1) {
            printPath(getNeighbour(d, stepDir, graph), graph);
            switch (stepDir)
            {
            case NORTH:
                dirToPrint = 'S';
                break;
            case EAST:
                dirToPrint = 'W';
                break;
            case SOUTH:
                dirToPrint = 'N';
                break;
            default:
                dirToPrint = 'E';
                break;
            }
            printf("%c", dirToPrint);
        }
    }
}

int main(int argc, char** argv) {
    FILE* inputFile;
    Graph graph;
    int source;
    int destination;
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
    source = getIndex(1, 1, &graph);
    destination = getIndex(graph.n - 2, graph.m - 2, &graph);
    printf("source: %d, dest:%d\n", source, destination);
    if (bfs(&graph, source, destination)) {
        printf("%d\n", graph.dist[destination]);
        printPath(destination, &graph);
    }

    /* Remember to free malloc-ed memory! */
    deleteGraph(&graph);
    return EXIT_SUCCESS;
}